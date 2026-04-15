# Installation of Windows on QEMU for TestDrive
Since the Windows image size is very large, we do not provide it separately, but we will provide a method for installation.

First, I recommand [Windows 11 IoT version](https://computernewb.com/isos/windows/) ISO file.
And please modify the `qemu_testdrive.ini` file as follows.

```ini
[QEMU]
...
BOOT_FROM_CDROM			= true
CDROM_IMAGE				= en-us_windows_11_iot_enterprise_ltsc_2024_x64_dvd_f6b14814.iso
...
```

Once setup reaches the language selection screen, press `Shift+F10` to open a command prompt. Run the following commands: 
```bash
> reg add HKLM\SYSTEM\Setup\LabConfig
> reg add HKLM\SYSTEM\Setup\LabConfig /t REG_DWORD /v BypassTPMCheck /d 1
> reg add HKLM\SYSTEM\Setup\LabConfig /t REG_DWORD /v BypassSecureBootCheck /d 1
> reg add HKLM\SYSTEM\Setup\LabConfig /t REG_DWORD /v BypassRAMCheck /d 1
> reg add HKLM\SYSTEM\Setup\LabConfig /t REG_DWORD /v BypassCPUCheck /d 1
```

You can now close command prompt and install Windows 11 as normal.


(Refer to : [How to install Windows 11 in QEMU](https://computernewb.com/wiki/QEMU/Guests/Windows_11))
