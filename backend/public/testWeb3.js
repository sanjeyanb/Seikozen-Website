import Web3 from 'web3';

const web3 = new Web3('http://localhost:7545'); // Ganache RPC URL

const contractAddress = "0x319B7d67a23635626751671F09fa03A5Ec772E46";
const abi = [ /* Add the updated ABI here */ ];

const contract = new web3.eth.Contract(abi, contractAddress);

web3.eth.getBlockNumber()
    .then(blockNumber => {
        console.log("Current block number: ", blockNumber);
    })
    .catch(err => {
        console.error("Error: ", err);
    });

contract.methods.productCount().call()
    .then(count => {
        console.log("Total products in the contract: ", count);
    })
    .catch(err => {
        console.error("Error fetching product count: ", err);
    });
