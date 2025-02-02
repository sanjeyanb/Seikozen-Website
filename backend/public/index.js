document.addEventListener('DOMContentLoaded', function () {
  if (window.ethereum) {
      const web3 = new Web3(window.ethereum);

      const contractAddress = "0x319B7d67a23635626751671F09fa03A5Ec772E46"; // Updated address
      const abi = [
          {
              "inputs": [
                  {
                      "internalType": "string",
                      "name": "_barcodeId",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "_name",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "_orderNo",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "_origin",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "_packingDate",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "_expiryDate",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "_lotNo",
                      "type": "string"
                  },
                  {
                      "internalType": "address",
                      "name": "_receiver",
                      "type": "address"
                  },
                  {
                      "internalType": "string",
                      "name": "_senderEmail",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "_receiverEmail",
                      "type": "string"
                  }
              ],
              "name": "addProduct",
              "outputs": [],
              "stateMutability": "nonpayable",
              "type": "function"
          },
          {
              "inputs": [],
              "name": "productCount",
              "outputs": [
                  {
                      "internalType": "uint256",
                      "name": "",
                      "type": "uint256"
                  }
              ],
              "stateMutability": "view",
              "type": "function"
          },
          {
              "inputs": [
                  {
                      "internalType": "uint256",
                      "name": "",
                      "type": "uint256"
                  }
              ],
              "name": "products",
              "outputs": [
                  {
                      "internalType": "uint256",
                      "name": "id",
                      "type": "uint256"
                  },
                  {
                      "internalType": "string",
                      "name": "barcodeId",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "name",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "orderNo",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "origin",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "packingDate",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "expiryDate",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "lotNo",
                      "type": "string"
                  },
                  {
                      "internalType": "address",
                      "name": "sender",
                      "type": "address"
                  },
                  {
                      "internalType": "address",
                      "name": "receiver",
                      "type": "address"
                  },
                  {
                      "internalType": "string",
                      "name": "senderEmail",
                      "type": "string"
                  },
                  {
                      "internalType": "string",
                      "name": "receiverEmail",
                      "type": "string"
                  }
              ],
              "stateMutability": "view",
              "type": "function"
          }
      ];

      const contract = new web3.eth.Contract(abi, contractAddress);

      document.getElementById('addProductForm').onsubmit = async (e) => {
          e.preventDefault();
          const barcodeId = document.getElementById('barcodeId').value.trim();
          const name = document.getElementById('productName').value.trim();
          const orderNo = document.getElementById('orderNo').value.trim();
          const origin = document.getElementById('origin').value.trim();
          const packingDate = document.getElementById('packingDate').value.trim();
          const expiryDate = document.getElementById('expiryDate').value.trim();
          const lotNo = document.getElementById('lotNo').value.trim();
          const receiver = document.getElementById('receiverAddress').value.trim();
          const senderEmail = document.getElementById('senderEmail').value.trim();
          const receiverEmail = document.getElementById('receiverEmail').value.trim();
      
          if (!barcodeId || !name || !orderNo || !origin || !packingDate || !expiryDate || !lotNo || !receiver || !senderEmail || !receiverEmail) {
              alert("All fields are required.");
              return;
          }
      
          if (!Web3.utils.isAddress(receiver)) {
              alert("Invalid Ethereum address for Receiver.");
              return;
          }
      
          try {
              const accounts = await web3.eth.getAccounts();
              await contract.methods.addProduct(
                  barcodeId,
                  name,
                  orderNo,
                  origin,
                  packingDate,
                  expiryDate,
                  lotNo,
                  receiver,
                  senderEmail,
                  receiverEmail
              ).send({ from: accounts[0] });
      
              alert("Product added successfully!");
          } catch (err) {
              console.error("Transaction error:", err);
              alert("Error adding product. Check the console for details.");
          }
      };
      };

  } else {
      alert("MetaMask not detected");
  }
});
