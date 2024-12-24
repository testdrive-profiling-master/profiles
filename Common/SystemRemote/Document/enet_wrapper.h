#ifndef __ENET_WRAPPER_H__
#define __ENET_WRAPPER_H__

#ifdef _WIN32
#include "STDInterface.h"
#else
#include <cstdio>
#endif
#include <enet/enet.h>
#ifdef __linux__
#include <cstdio>
#endif

typedef enum {
    ENET_STATUS_TIMEOUT = 0,
    ENET_STATUS_CONNECT,
    ENET_STATUS_DISCONNECT,
    ENET_STATUS_RECEIVE,
} ENET_STATUS;

class enet_common {
   public:
    enet_common(uint16_t port, size_t peer_count, size_t channel_count);
    virtual ~enet_common(void);

	ENET_STATUS DoService(uint32_t timeout = 5000);
	virtual bool OnConnect(ENetEvent& event);
	virtual void OnDisconnect(ENetEvent& event);
	virtual void OnReceive(ENetEvent& event);

    bool Send(ENetPeer* peer, uint8_t channel_id, const void* pData, size_t dwSize);

   protected:
    ENetHost* m_pHost;
};

class enet_server : public enet_common {
   public:
    enet_server(uint16_t port, size_t peer_count, size_t channel_count);
    virtual ~enet_server(void);
};

class enet_client : public enet_common {
   public:
    enet_client(const char* hostname, uint16_t port, size_t channel_count);
    virtual ~enet_client(void);

    bool Send(uint8_t channel_id, const void* pData, size_t dwSize);

   protected:
    ENetPeer* m_pPeer;
};

#endif  //__ENET_WRAPPER_H__
