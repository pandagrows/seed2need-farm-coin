SEED2NEED Core
=============

Setup
---------------------
[SEED2NEED Core](http://seed2need.org/wallet) is the original SEED2NEED client and it builds the backbone of the network. However, it downloads and stores the entire history of SEED2NEED transactions; depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more. Thankfully you only have to do this once.

Running
---------------------
The following are some helpful notes on how to run SEED2NEED Core on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/seed2need-qt` (GUI) or
- `bin/seed2needd` (headless)

### Windows

Unpack the files into a directory, and then run seed2need-qt.exe.

### macOS

Drag SEED2NEED-Qt to your applications folder, and then run SEED2NEED-Qt.

### Need Help?

* See the documentation at the [SEED2NEED Wiki](https://github.com/SEED2NEED-Project/SEED2NEED/wiki)
for help and more information.
* Ask for help on [BitcoinTalk](https://bitcointalk.org/index.php?topic=1262920.0) or on the [SEED2NEED Forum](http://forum.seed2need.org/).
* Join our Discord server [Discord Server](https://discord.seed2need.org)

Building
---------------------
The following are developer notes on how to build SEED2NEED Core on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [Gitian Building Guide](gitian-building.md)

Development
---------------------
The SEED2NEED repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Multiwallet Qt Development](multiwallet-qt.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://www.fuzzbawls.pw/seed2need/doxygen/)
- [Translation Process](translation_process.md)
- [Unit Tests](unit-tests.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Dnsseed Policy](dnsseed-policy.md)

### Resources
* Discuss on the [BitcoinTalk](https://bitcointalk.org/index.php?topic=1262920.0) or the [SEED2NEED](http://forum.seed2need.org/) forum.
* Join the [SEED2NEED Discord](https://discord.seed2need.org).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [Files](files.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.
