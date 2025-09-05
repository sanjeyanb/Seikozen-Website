// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract SupplyChain {
    struct Product {
        uint id;
        string barcodeId;
        string name;
        string orderNo;
        string origin;
        string packingDate;
        string expiryDate;
        string lotNo;
        address sender;
        address receiver;
        string senderEmail;
        string receiverEmail;
    }

    mapping(uint => Product) public products;
    uint public productCount;

    event ProductAdded(uint id, string name, address indexed sender, address indexed receiver);

    function addProduct(
        string memory _barcodeId,
        string memory _name,
        string memory _orderNo,
        string memory _origin,
        string memory _packingDate,
        string memory _expiryDate,
        string memory _lotNo,
        address _receiver,
        string memory _senderEmail,
        string memory _receiverEmail
    ) public {
        productCount++;
        products[productCount] = Product(
            productCount,
            _barcodeId,
            _name,
            _orderNo,
            _origin,
            _packingDate,
            _expiryDate,
            _lotNo,
            msg.sender,
            _receiver,
            _senderEmail,
            _receiverEmail
        );

        emit ProductAdded(productCount, _name, msg.sender, _receiver);
    }
}
