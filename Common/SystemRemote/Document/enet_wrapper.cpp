#include "enet_wrapper.h"

#ifdef USE_ZSTD
#include <zstd.h>
#endif

static int __ref_count	= 0;
//#define USE_ENET_COMPRESSION
#define	MIN_COMPRESSION_SIZE	(1024 * 16)		// minimum compression size : 16KBytes

#ifdef USE_ZSTD
// compression & decompression to increase enet bandwidth
static struct EnetComp : ENetCompressor {
	size_t ENET_CALLBACK compress(
			void * context, const ENetBuffer * inBuffers, size_t inBufferCount,
			size_t inLimit, enet_uint8 * outData, size_t outLimit) {
		if(inLimit >= MIN_COMPRESSION_SIZE) {
			size_t			iDestByteSize	= ZSTD_compressBound(inLimit);
			unsigned char*	pComp			= new unsigned char[iDestByteSize];
			ZSTD_CCtx*		cctx			= ZSTD_createCCtx();
			ZSTD_outBuffer	output			= { pComp, iDestByteSize, 0 };
			ZSTD_CCtx_setParameter(cctx, ZSTD_c_compressionLevel, 3);
			ZSTD_CCtx_setParameter(cctx, ZSTD_c_checksumFlag, 1);

			for(int i=0;i<inBufferCount;i++) {
				ZSTD_inBuffer input		= { inBuffers->data, inBuffers->dataLength, 0 };
				size_t const remaining	= ZSTD_compressStream2(cctx, &output , &input, (i == (inBufferCount-1)) ? ZSTD_e_end : ZSTD_e_continue);
				if(input.pos != input.size){
					printf("*E: compression is failed.\n");
					output.pos	= 0;
					break;
				}
				inBuffers++;
			}
			ZSTD_freeCCtx(cctx);
			return output.pos;
		}
		return 0;
	}

	size_t ENET_CALLBACK decompress(
			void * context, const enet_uint8 * inData, size_t inLimit,
			enet_uint8 * outData, size_t outLimit) {
		ZSTD_inBuffer		input			= { inData, inLimit, 0 };
		ZSTD_outBuffer		output			= { outData, outLimit, 0 };
		ZSTD_DCtx* const	dctx			= ZSTD_createDCtx();
		if(dctx) {
			size_t const ret = ZSTD_decompressStream(dctx, &output, &input);
			if(input.pos != input.size) {
				printf("*E: decompression is failed.\n");
				output.pos	= 0;
			}
			ZSTD_freeDCtx(dctx);
			return input.pos;
		}
		return 0;

	}

	void ENET_CALLBACK destroy(void * context) {
	}
} __EnetComp;
#endif

enet_common::enet_common(uint16_t port, size_t peer_count, size_t channel_count) {
	if(!__ref_count) {
		if(enet_initialize()) {
			printf("*E: enet library is not initialized.\n");
			exit(1);
		}
	}
	__ref_count++;

	if(port) {
		// server
		ENetAddress		address;
		address.host 	= ENET_HOST_ANY;
		address.port 	= port;
		m_pHost			= enet_host_create(&address, peer_count, channel_count, 0, 0);
	} else {
		// client
		m_pHost			= enet_host_create(NULL, peer_count, channel_count, 0, 0);
	}

	if(!m_pHost) {
		printf("*E: Can't create host.\n");
		exit(1);
	}

#ifdef USE_ENET_COMPRESSION
	enet_host_compress(m_pHost, &__EnetComp);
#endif
}

enet_common::~enet_common(void) {
	if(m_pHost) {
		enet_host_flush(m_pHost);
		enet_host_destroy(m_pHost);
		m_pHost	= NULL;
	}

	__ref_count--;
	if(!__ref_count) {
		enet_deinitialize();
	}
}

ENET_STATUS enet_common::DoService(uint32_t timeout) {
	ENET_STATUS		status	= ENET_STATUS_TIMEOUT;
	ENetEvent		event;
	if(enet_host_service(m_pHost, &event, timeout) > 0) {
		status = (ENET_STATUS)event.type;
		switch(status) {
		case ENET_STATUS_CONNECT:
			if(!OnConnect(event)) {
				enet_peer_disconnect(event.peer, 0);
			}
			break;
		case ENET_STATUS_DISCONNECT:
			OnDisconnect(event);
			break;
		case ENET_STATUS_RECEIVE:
			OnReceive(event);
			enet_packet_destroy(event.packet);
			break;
		}
	}
	return status;
}

bool enet_common::Send(ENetPeer * peer, uint8_t channel_id, const void* pData, size_t dwSize)
{
	if(peer) {
		ENetPacket* packet = enet_packet_create(pData, dwSize, ENET_PACKET_FLAG_RELIABLE);
		if(!enet_peer_send(peer, channel_id, packet)) {
			enet_host_flush(m_pHost);
			return true;
		}
	}
	return false;
}

bool enet_common::OnConnect(ENetEvent& event) {return true;}
void enet_common::OnDisconnect(ENetEvent& event) {}
void enet_common::OnReceive(ENetEvent& event) {}

enet_server::enet_server(uint16_t port, size_t peer_count, size_t channel_count) : enet_common(port, peer_count, channel_count) {
}

enet_server::~enet_server(void){
}

enet_client::enet_client(const char* hostname, uint16_t port, size_t channel_count) : enet_common(0, 1, channel_count){
	ENetAddress		address;
	enet_address_set_host (&address, hostname);
	address.port	= port;
	m_pPeer			= enet_host_connect(m_pHost, &address, channel_count, 0);

	if(!m_pPeer) {
		printf("*E: No server found.\n");
		exit(1);
	}

	if(DoService(5000) != ENET_STATUS_CONNECT) {
		printf("*E: No connection to server.\n");
		exit(1);
	}
}

enet_client::~enet_client(void){
	if(m_pPeer) {
		if(m_pHost) enet_host_flush(m_pHost);
		enet_peer_disconnect_now(m_pPeer, 0);
		m_pPeer	= NULL;
	}
}

bool enet_client::Send(uint8_t channel_id, const void* pData, size_t dwSize){
	return enet_common::Send(m_pPeer, channel_id, pData, dwSize);
}
