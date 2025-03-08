#include <ESP8266WiFi.h>  // Gunakan <WiFi.h> jika memakai ESP32
#include <FirebaseESP8266.h> // Gunakan FirebaseESP32.h jika pakai ESP32
// #include "TokenHelper.h"  // Tambahkan TokenHelper untuk menangani token

#define WIFI_SSID "sagasitas_2.4G"    
#define WIFI_PASSWORD "rahasia1"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

const int soilPin = A0;
const int relayPin = D7;

int lastMoisture = -1; // Variabel untuk menyimpan kelembaban terakhir

void setup() {
    Serial.begin(115200);

    // Koneksi ke WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected!");

    // Konfigurasi Firebase
    config.api_key = "AIzaSyBNsATavK8A4MxNs4ehSpyMwx-FFfqs3uI"; 
    config.database_url = "https://ageraproject1-default-rtdb.asia-southeast1.firebasedatabase.app/";  

    // Gunakan token Firebase tanpa email/password
    config.signer.tokens.legacy_token = "qGRYZXhlx927UCjdyHF5HqevPbrK6mgQYe6M4jmY"; // Ganti dengan Secret Key dari Firebase

    // Inisialisasi Firebase
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    Serial.println("Mengambil database URL dari Firebase...");
    while (config.database_url == "") {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nFirebase Database URL: " + String(config.database_url.c_str()));

    pinMode(soilPin, INPUT);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, HIGH); // Matikan pompa di awal
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi terputus, mencoba reconnect...");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        delay(5000);
        return;
    }

    if (!Firebase.ready()) {
        Serial.println("Firebase tidak siap, menunggu...");
        delay(5000);
        return;
    }

    int soilValue = analogRead(soilPin);
    int moisturePercent = map(soilValue, 1023, 0, 0, 100);

    Serial.print("Soil Moisture: ");
    Serial.print(moisturePercent);
    Serial.println("%");

    // Kirim data ke Firebase hanya jika ada perubahan
    if (moisturePercent != lastMoisture) {
        if (Firebase.setInt(firebaseData, "/tanaman/moisture", moisturePercent)) {
            Serial.println("Data terkirim ke Firebase");
        } else {
            Serial.print("Gagal mengirim data: ");
            Serial.println(firebaseData.errorReason().c_str());
        }
        lastMoisture = moisturePercent;
    }

    // Logika penyiraman otomatis
    if (moisturePercent < 40) {
        digitalWrite(relayPin, LOW); 
        Firebase.setString(firebaseData, "/tanaman/status", "Menyiram");
        Firebase.setInt(firebaseData, "/tanaman/moisture", moisturePercent);
    } else {
        digitalWrite(relayPin, HIGH);
        Firebase.setString(firebaseData, "/tanaman/status", "Tidak menyiram");
        Firebase.setInt(firebaseData, "/tanaman/moisture", moisturePercent);
    }

    delay(5000); 
}