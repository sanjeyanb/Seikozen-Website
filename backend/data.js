const express = require('express');
const mysql = require('mysql');
const app = express();

// Set up MySQL connection
const db = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: 'Vidhyasri@06',
  database: 'pharma_supply_chain'
});

// Connect to the database
db.connect((err) => {
  if (err) {
    console.log('Error connecting to the database:', err);
    return;
  }
  console.log('Connected to the database');
});

// Serve static files like CSS, JS, etc.
app.use(express.static('public'));

// Route to fetch the products data as JSON
app.get('/api/products', (req, res) => {
  const query = 'SELECT * FROM add_products';  // Query to get products from DB
  db.query(query, (err, results) => {
    if (err) {
      console.log('Error fetching data:', err);
      res.status(500).send('Error fetching data');
      return;
    }
    res.json(results);  // Send the products data as JSON
  });
});

// Start the server
const port = 3001;
app.listen(port, () => {
  console.log(`Server running on http://localhost:${port}`);
});
