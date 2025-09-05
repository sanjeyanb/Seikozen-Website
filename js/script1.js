// Simulated user registration and lookup with roles
const users = [
    { username: "admin", password: "admin123", role: "admin" },
    { username: "manufacturer", password: "manu123", role: "manufacturer" },
    { username: "retailer", password: "retail123", role: "retailer" },
    { username: "distributor", password: "dist123", role: "distributor" },
];

document.addEventListener("DOMContentLoaded", () => {
    const loginForm = document.querySelector("#loginForm");

    if (loginForm) {
        loginForm.addEventListener("submit", (event) => {
            event.preventDefault(); // Prevent the default form submission

            const username = document.querySelector("#username").value.trim();
            const password = document.querySelector("#password").value;

            // Find the user
            const user = users.find(
                (u) => u.username === username && u.password === password
            );

            if (user) {
                alert(`Welcome, ${user.username}!`);
                
                // Redirect based on role
                switch (user.role) {
                    case "admin":
                        window.location.href = "new.html";
                        break;
                    case "manufacturer":
                        window.location.href = "manufacturer.html";
                        break;
                    case "retailer":
                        window.location.href = "retailer.html";
                        break;
                    case "distributor":
                        window.location.href = "distributor.html";
                        break;
                    default:
                        alert("Role not recognized.");
                        break;
                }
            } else {
                alert("Invalid username or password."); // Show an error alert
            }
        });
    }
});
