import { initializeApp } from "https://www.gstatic.com/firebasejs/11.4.0/firebase-app.js";
        import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/11.4.0/firebase-database.js";

        const firebaseConfig = {
            apiKey: "AIzaSyBNsATavK8A4MxNs4ehSpyMwx-FFfqs3uI",
            authDomain: "ageraproject1.firebaseapp.com",
            projectId: "ageraproject1",
            storageBucket: "ageraproject1.firebasestorage.app",
            messagingSenderId: "117896735053",
            appId: "1:117896735053:web:cdcaa9a74882314036af04",
            measurementId: "G-7YCTF3PZJ1",
            databaseURL: "https://ageraproject1-default-rtdb.asia-southeast1.firebasedatabase.app"  // <-- Perbarui URL
        };
        
        const app = initializeApp(firebaseConfig);
        const database = getDatabase(app);
        

        const moistureRef = ref(database, 'tanaman/moisture');
        const statusRef = ref(database, 'tanaman/status');

        onValue(moistureRef, (snapshot) => {
            let moisture = snapshot.val();
            if (moisture !== null) {
                document.getElementById("moisture").innerText = moisture + "%";
            } else {
                console.log("Data kelembaban tidak tersedia");
            }
        });
        
        onValue(statusRef, (snapshot) => {
            let status = snapshot.val();
            let statusElem = document.getElementById("status");
            if (status !== null) {
                statusElem.innerText = status;
                statusElem.className = status === "Menyiram" ? "badge bg-success" : "badge bg-secondary";
            } else {
                console.log("Data status tidak tersedia");
            }
        });
        