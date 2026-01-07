#define SSIDa ssid_from_AP_4 // if your key is a string it needs to be in quotes. Note no semicolon after a hash define
//#define SSID "Sloan-Home" // if your key is a string it needs to be in quotes. Note no semicolon after a hash define
                           // using the define of ssid conflicts with the WiFi library WiFi.SSID
//#define ssid "XXXXXXX"    // this does not work
#define PASSWORDa your_password_for_AP_4

// the following block is for WiFiMulti.h support on the ESP32. You can define as many as you choose.
#define ssid_from_AP_1 "IBE"
#define your_password_for_AP_1 "ibeinc9121"
#define ssid_from_AP_2 "IBE_BATTERY"
#define your_password_for_AP_2 "ibeinc9121"
#define ssid_from_AP_3 "Sloan-Home"
#define your_password_for_AP_3 "04052002"
#define ssid_from_AP_4 "IBE-OfficeTrendnet"
#define your_password_for_AP_4 "ibeinc9121"

#define sql_server_address "192.168.0.85"
#define sql_server_insert_name "pi_insert"
#define sql_server_insert_password "raspbery"

// WiFi AP SSID
#define WIFI_SSID "IBE"
// WiFi password
#define WIFI_PASSWORD "ibeinc9121"
// InfluxDB v2 server url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
#define INFLUXDB_URL "http://ubuntuderek.ibeinc.net:8086"
//#define INFLUXDB_URL "http://192.168.0.130:8086"
//#define INFLUXDB_URL "http://ubuntuderek.ibe-inc.local:8086" //not working
// InfluxDB v2 server or cloud API token (Use: InfluxDB UI -> Data -> API Tokens -> <select token>)
#define INFLUXDB_TOKEN "9bd64c8a047b7b9c2622c0e0222bcf90450a1d402c1fde4a104773481da3517e"
