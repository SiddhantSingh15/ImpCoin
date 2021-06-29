# ImpCoin

A simple cryptocurrency implementation based on a proof-of-work blockchain, written in C.

## Dependencies
ImpCoin uses [nng](https://github.com/nanomsg/nng) for its network layer, [binn](https://github.com/liteserver/binn) for serialising / deserialising, and [libsodium](https://github.com/jedisct1/libsodium) for hashing.

## Usage
To get started, install the libraries according to the instructions in the linked repos.
After this, run

```shell
$ make all
$ ./address_node
```

This will build the project, and start the `address_node`, which allows the nodes on the network to communicate. Unfortunately, ImpCoin isn't fully decentralised (yet).
Then, you can start up nodes like so:

```shell
$ ./node
```
You will be prompted to enter a username and port number. Make sure not to use `127.0.0.1:8000`, as the `address_node` uses it by default.

## Commands

After starting up a node, you can use these commands:

`Imp> t` - to make a transaction
`Imp> m` - to start mining
`Imp> p bc` - to print your local blockchain
`Imp> p latest_block` - to print the latest block on your local blockchain
`Imp> p mempool` - to print your local mempool (list of pending transactions)
`Imp> q` - to quit
