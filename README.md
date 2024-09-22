# nconnect
C tool to facilitate the connection to networks from the terminal.
This will edit the file /etc/network/interfaces to put the correct wireless network information inside.

# Installation
<ol>
<li>Clone the master branch of this repository</li>
	
```bash
git clone https://github.com/arkneid/nconnect.git
```
<li>Enter the nconnect folder and run make command</li>

```bash
cd nconnect
make
```
<li>Now install nconnect tool</li>	

```bash
sudo make clean install
```
</ol>

# Uninstall
<ol>
<li>Go to the previous clone git folder</li>

```bash
cd nconnect
sudo make uninstall
```
</ol>

# Usage
Normal usage:

```bash
sudo nconnect -s <network_ssid> -w <wireless_network_interface> -p <network_password>
sudo ifdown <wireless_network_interface>
sudo ifup <wireless_network_interface>
```

Print help:

```bash
sudo nconnect -h or sudo nconnect --help
```

# Author
arknied(https://github.com/arkneid)

# License
This software is released under the MIT License, see LICENSE.
