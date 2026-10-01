# Installation of Ubuntu on QEMU for TestDrive
Since the Ubuntu image size is very large, we do not provide it separately, but we will provide a method for installation.
First, download the desired [Ubuntu](https://ubuntu.com/download/desktop) (or another version of the Linux image) ISO file.
And copy .iso file on current directory.

Now, run QEMU to install it.
```bash
> qemu_testdrive
```

* Note : If you do not have an internet connection, you may need to change your DNS settings to `8.8.8.8`.

You need to install the QEMU agent for Ubuntu.
```bash
> sudo apt update
> sudo apt upgrade
> sudo apt install qemu-guest-agent
> sudo systemctl enable --now qemu-guest-agent
```

And you can keep the image size small by using the following command.
```bash
> sudo apt purge libreoffice* -y
> sudo apt purge thunderbird* -y
> sudo apt autoremove --purge -y
```

# Installation of Windows on QEMU for TestDrive
Since the Windows image size is very large, we do not provide it separately, but we will provide a method for installation.

First, I recommand [Windows 11 IoT version](https://computernewb.com/isos/windows/) ISO file.
And copy .iso file on current directory.

* Note : Windows 11 has requirements such as TPM 2.0, necessitating a process to bypass them.
         If you install a different version of Windows, you need to create a new .iso image
	     with [this script](https://github.com/ntdevlabs/nano11).

* Summation:
Once setup reaches the language selection screen, press `Shift+F10` to open a command prompt. Run the following commands: 
```bash
> reg add HKLM\SYSTEM\Setup\LabConfig
> reg add HKLM\SYSTEM\Setup\LabConfig /t REG_DWORD /v BypassTPMCheck /d 1
> reg add HKLM\SYSTEM\Setup\LabConfig /t REG_DWORD /v BypassSecureBootCheck /d 1
> reg add HKLM\SYSTEM\Setup\LabConfig /t REG_DWORD /v BypassRAMCheck /d 1
> reg add HKLM\SYSTEM\Setup\LabConfig /t REG_DWORD /v BypassCPUCheck /d 1
```

You can now close command prompt and install Windows 11 as normal.

Refer to : [How to install Windows 11 in QEMU](https://computernewb.com/wiki/QEMU/Guests/Windows_11)
           [guest agent for windows](https://fedorapeople.org/groups/virt/virtio-win/direct-downloads/archive-virtio)
		   [Windows deloat](https://github.com/Raphire/Win11Debloat) for light-weight installation & high performance.
