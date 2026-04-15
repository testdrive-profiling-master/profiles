# Installation of Ubuntu on QEMU for TestDrive
Since the Ubuntu image size is very large, we do not provide it separately, but we will provide a method for installation.
First, download the desired [Ubuntu](https://ubuntu.com/download/desktop) (or another version of the Linux image) ISO file.
And please modify the `qemu_testdrive.ini` file as follows.

```ini
[QEMU]
...
BOOT_FROM_CDROM			= true
CDROM_IMAGE				= ubuntu-24.04.4-desktop-amd64.iso (path to the installation ISO file you downloaded)
...
```

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