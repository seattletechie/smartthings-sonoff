#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>

#include "Sonoff.h"

const char *ssid = "<WiFi>";
const char *password = "<Password>";

const char sonoffLine_html[] = {
  0x3c, 0x68, 0x74, 0x6d, 0x6c, 0x3e, 0x0a, 0x3c, 0x68, 0x65, 0x61, 0x64,
  0x3e, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x3c, 0x74, 0x69, 0x74, 0x6c, 0x65,
  0x3e, 0x53, 0x6d, 0x61, 0x72, 0x74, 0x54, 0x68, 0x69, 0x6e, 0x67, 0x73,
  0x20, 0x53, 0x6f, 0x6e, 0x6f, 0x66, 0x66, 0x20, 0x42, 0x61, 0x73, 0x69,
  0x63, 0x3c, 0x2f, 0x74, 0x69, 0x74, 0x6c, 0x65, 0x3e, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x3c, 0x73, 0x74, 0x79, 0x6c, 0x65, 0x3e, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x62, 0x6f, 0x64, 0x79, 0x20, 0x7b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x62, 0x61, 0x63, 0x6b, 0x67, 0x72,
  0x6f, 0x75, 0x6e, 0x64, 0x2d, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x20,
  0x23, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3b, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6f, 0x6e, 0x74, 0x2d, 0x66, 0x61,
  0x6d, 0x69, 0x6c, 0x79, 0x3a, 0x20, 0x41, 0x72, 0x69, 0x61, 0x6c, 0x2c,
  0x20, 0x48, 0x65, 0x6c, 0x76, 0x65, 0x74, 0x69, 0x63, 0x61, 0x2c, 0x20,
  0x53, 0x61, 0x6e, 0x73, 0x2d, 0x53, 0x65, 0x72, 0x69, 0x66, 0x3b, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x43, 0x6f, 0x6c, 0x6f,
  0x72, 0x3a, 0x20, 0x23, 0x30, 0x30, 0x30, 0x30, 0x38, 0x38, 0x3b, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x7d, 0x20, 0x20, 0x20, 0x20, 0x3c, 0x2f, 0x73,
  0x74, 0x79, 0x6c, 0x65, 0x3e, 0x0a, 0x3c, 0x2f, 0x68, 0x65, 0x61, 0x64,
  0x3e, 0x0a, 0x3c, 0x62, 0x6f, 0x64, 0x79, 0x3e, 0x3c, 0x68, 0x31, 0x3e,
  0x53, 0x6d, 0x61, 0x72, 0x74, 0x54, 0x68, 0x69, 0x6e, 0x67, 0x73, 0x20,
  0x53, 0x6f, 0x6e, 0x6f, 0x66, 0x66, 0x20, 0x42, 0x61, 0x73, 0x69, 0x63,
  0x21, 0x3c, 0x2f, 0x68, 0x31, 0x3e, 0x0a, 0x3c, 0x70, 0x20, 0x69, 0x64,
  0x3d, 0x22, 0x75, 0x70, 0x74, 0x69, 0x6d, 0x65, 0x22, 0x3e, 0x55, 0x70,
  0x74, 0x69, 0x6d, 0x65, 0x3a, 0x20, 0x3c, 0x2f, 0x70, 0x3e, 0x0a, 0x3c,
  0x70, 0x20, 0x69, 0x64, 0x3d, 0x22, 0x72, 0x65, 0x6c, 0x61, 0x79, 0x22,
  0x3e, 0x52, 0x65, 0x6c, 0x61, 0x79, 0x3c, 0x2f, 0x70, 0x3e, 0x0a, 0x3c,
  0x70, 0x20, 0x69, 0x64, 0x3d, 0x22, 0x77, 0x69, 0x66, 0x69, 0x22, 0x3e,
  0x57, 0x69, 0x46, 0x69, 0x3a, 0x3c, 0x2f, 0x70, 0x3e, 0x0a, 0x3c, 0x70,
  0x20, 0x69, 0x64, 0x3d, 0x22, 0x69, 0x70, 0x22, 0x3e, 0x49, 0x70, 0x3a,
  0x3c, 0x2f, 0x70, 0x3e, 0x0a, 0x3c, 0x70, 0x20, 0x69, 0x64, 0x3d, 0x22,
  0x6d, 0x61, 0x63, 0x22, 0x3e, 0x4d, 0x41, 0x63, 0x3a, 0x3c, 0x2f, 0x70,
  0x3e, 0x0a, 0x3c, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x61, 0x63, 0x74, 0x69,
  0x6f, 0x6e, 0x3d, 0x22, 0x2f, 0x74, 0x6f, 0x67, 0x67, 0x6c, 0x65, 0x22,
  0x20, 0x6d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x3d, 0x22, 0x70, 0x6f, 0x73,
  0x74, 0x22, 0x3e, 0x3c, 0x69, 0x6e, 0x70, 0x75, 0x74, 0x20, 0x74, 0x79,
  0x70, 0x65, 0x3d, 0x22, 0x73, 0x75, 0x62, 0x6d, 0x69, 0x74, 0x22, 0x20,
  0x76, 0x61, 0x6c, 0x75, 0x65, 0x3d, 0x22, 0x54, 0x6f, 0x67, 0x67, 0x6c,
  0x65, 0x20, 0x52, 0x65, 0x6c, 0x61, 0x79, 0x22, 0x3e, 0x3c, 0x2f, 0x66,
  0x6f, 0x72, 0x6d, 0x3e, 0x0a, 0x3c, 0x62, 0x72, 0x2f, 0x3e, 0x0a, 0x3c,
  0x62, 0x72, 0x2f, 0x3e, 0x0a, 0x3c, 0x64, 0x69, 0x76, 0x20, 0x69, 0x64,
  0x3d, 0x22, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x22, 0x3e, 0x0a, 0x0a,
  0x3c, 0x2f, 0x64, 0x69, 0x76, 0x3e, 0x0a, 0x3c, 0x2f, 0x62, 0x6f, 0x64,
  0x79, 0x3e, 0x0a, 0x3c, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74, 0x3e, 0x0a,
  0x20, 0x20, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x73,
  0x74, 0x61, 0x74, 0x65, 0x28, 0x73, 0x75, 0x63, 0x63, 0x65, 0x73, 0x73,
  0x2c, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x29, 0x20, 0x7b, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x72, 0x65, 0x71, 0x20, 0x3d,
  0x20, 0x6e, 0x65, 0x77, 0x20, 0x58, 0x4d, 0x4c, 0x48, 0x74, 0x74, 0x70,
  0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x28, 0x29, 0x3b, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x72, 0x65, 0x71, 0x2e, 0x6f, 0x70, 0x65, 0x6e, 0x28,
  0x27, 0x47, 0x45, 0x54, 0x27, 0x2c, 0x20, 0x27, 0x2f, 0x73, 0x74, 0x61,
  0x74, 0x65, 0x27, 0x2c, 0x20, 0x74, 0x72, 0x75, 0x65, 0x29, 0x3b, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x71, 0x2e, 0x73, 0x65, 0x74, 0x52,
  0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72,
  0x28, 0x27, 0x41, 0x63, 0x63, 0x65, 0x70, 0x74, 0x27, 0x2c, 0x20, 0x27,
  0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f,
  0x6a, 0x73, 0x6f, 0x6e, 0x27, 0x29, 0x3b, 0x0a, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x72, 0x65, 0x71, 0x2e, 0x6f, 0x6e, 0x72, 0x65, 0x61, 0x64, 0x79,
  0x73, 0x74, 0x61, 0x74, 0x65, 0x63, 0x68, 0x61, 0x6e, 0x67, 0x65, 0x20,
  0x3d, 0x20, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x28,
  0x29, 0x20, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x69, 0x66,
  0x20, 0x28, 0x72, 0x65, 0x71, 0x2e, 0x72, 0x65, 0x61, 0x64, 0x79, 0x53,
  0x74, 0x61, 0x74, 0x65, 0x20, 0x3d, 0x3d, 0x20, 0x34, 0x29, 0x20, 0x7b,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x69, 0x66, 0x20,
  0x28, 0x72, 0x65, 0x71, 0x2e, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x20,
  0x3d, 0x3d, 0x20, 0x32, 0x30, 0x30, 0x29, 0x20, 0x7b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x73, 0x75, 0x63, 0x63,
  0x65, 0x73, 0x73, 0x28, 0x72, 0x65, 0x71, 0x2e, 0x72, 0x65, 0x73, 0x70,
  0x6f, 0x6e, 0x73, 0x65, 0x54, 0x65, 0x78, 0x74, 0x29, 0x3b, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x20, 0x65, 0x6c, 0x73,
  0x65, 0x20, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x28, 0x29, 0x3b, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x7d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x3b, 0x0a,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x71, 0x2e, 0x73, 0x65, 0x6e,
  0x64, 0x28, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x7d, 0x0a, 0x0a, 0x20, 0x20,
  0x73, 0x74, 0x61, 0x74, 0x65, 0x28, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69,
  0x6f, 0x6e, 0x20, 0x28, 0x64, 0x61, 0x74, 0x61, 0x29, 0x20, 0x7b, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x6a, 0x73, 0x6f, 0x6e,
  0x20, 0x3d, 0x20, 0x4a, 0x53, 0x4f, 0x4e, 0x2e, 0x70, 0x61, 0x72, 0x73,
  0x65, 0x28, 0x64, 0x61, 0x74, 0x61, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x76, 0x61, 0x72, 0x20, 0x75, 0x70, 0x74, 0x69, 0x6d, 0x65, 0x50,
  0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e,
  0x67, 0x65, 0x74, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x42, 0x79,
  0x49, 0x64, 0x28, 0x27, 0x75, 0x70, 0x74, 0x69, 0x6d, 0x65, 0x27, 0x29,
  0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x75, 0x70, 0x74, 0x69, 0x6d, 0x65,
  0x50, 0x2e, 0x69, 0x6e, 0x6e, 0x65, 0x72, 0x54, 0x65, 0x78, 0x74, 0x3d,
  0x27, 0x55, 0x70, 0x74, 0x69, 0x6d, 0x65, 0x3a, 0x27, 0x2b, 0x6a, 0x73,
  0x6f, 0x6e, 0x2e, 0x75, 0x70, 0x74, 0x69, 0x6d, 0x65, 0x2f, 0x31, 0x30,
  0x30, 0x30, 0x2b, 0x27, 0x20, 0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x73,
  0x27, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x72,
  0x65, 0x6c, 0x61, 0x79, 0x50, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75,
  0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x67, 0x65, 0x74, 0x45, 0x6c, 0x65, 0x6d,
  0x65, 0x6e, 0x74, 0x42, 0x79, 0x49, 0x64, 0x28, 0x27, 0x72, 0x65, 0x6c,
  0x61, 0x79, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x72, 0x65,
  0x6c, 0x61, 0x79, 0x50, 0x2e, 0x69, 0x6e, 0x6e, 0x65, 0x72, 0x54, 0x65,
  0x78, 0x74, 0x3d, 0x27, 0x52, 0x65, 0x6c, 0x61, 0x79, 0x3a, 0x27, 0x2b,
  0x6a, 0x73, 0x6f, 0x6e, 0x2e, 0x72, 0x65, 0x6c, 0x61, 0x79, 0x3b, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x77, 0x69, 0x66, 0x69,
  0x50, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74,
  0x2e, 0x67, 0x65, 0x74, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x42,
  0x79, 0x49, 0x64, 0x28, 0x27, 0x77, 0x69, 0x66, 0x69, 0x27, 0x29, 0x3b,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x77, 0x69, 0x66, 0x69, 0x50, 0x2e, 0x69,
  0x6e, 0x6e, 0x65, 0x72, 0x54, 0x65, 0x78, 0x74, 0x3d, 0x27, 0x53, 0x73,
  0x69, 0x64, 0x3a, 0x27, 0x2b, 0x6a, 0x73, 0x6f, 0x6e, 0x2e, 0x73, 0x73,
  0x69, 0x64, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20,
  0x69, 0x70, 0x50, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65,
  0x6e, 0x74, 0x2e, 0x67, 0x65, 0x74, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e,
  0x74, 0x42, 0x79, 0x49, 0x64, 0x28, 0x27, 0x69, 0x70, 0x27, 0x29, 0x3b,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x69, 0x70, 0x50, 0x2e, 0x69, 0x6e, 0x6e,
  0x65, 0x72, 0x54, 0x65, 0x78, 0x74, 0x3d, 0x27, 0x69, 0x70, 0x3a, 0x27,
  0x2b, 0x6a, 0x73, 0x6f, 0x6e, 0x2e, 0x69, 0x70, 0x3b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x6d, 0x61, 0x63, 0x50, 0x20, 0x3d,
  0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x67, 0x65,
  0x74, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x42, 0x79, 0x49, 0x64,
  0x28, 0x27, 0x6d, 0x61, 0x63, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x6d, 0x61, 0x63, 0x50, 0x2e, 0x69, 0x6e, 0x6e, 0x65, 0x72, 0x54,
  0x65, 0x78, 0x74, 0x3d, 0x27, 0x6d, 0x61, 0x63, 0x3a, 0x27, 0x2b, 0x6a,
  0x73, 0x6f, 0x6e, 0x2e, 0x6d, 0x61, 0x63, 0x3b, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x76, 0x61, 0x72, 0x20, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x44,
  0x69, 0x76, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e,
  0x74, 0x2e, 0x67, 0x65, 0x74, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74,
  0x42, 0x79, 0x49, 0x64, 0x28, 0x27, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67,
  0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x77, 0x68, 0x69, 0x6c,
  0x65, 0x20, 0x28, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x44, 0x69, 0x76,
  0x2e, 0x66, 0x69, 0x72, 0x73, 0x74, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x29,
  0x20, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x63, 0x6f, 0x6e,
  0x66, 0x69, 0x67, 0x44, 0x69, 0x76, 0x2e, 0x72, 0x65, 0x6d, 0x6f, 0x76,
  0x65, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28, 0x63, 0x6f, 0x6e, 0x66, 0x69,
  0x67, 0x44, 0x69, 0x76, 0x2e, 0x66, 0x69, 0x72, 0x73, 0x74, 0x43, 0x68,
  0x69, 0x6c, 0x64, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x66, 0x20, 0x3d, 0x20,
  0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65,
  0x61, 0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27,
  0x66, 0x6f, 0x72, 0x6d, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20,
  0x66, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75,
  0x74, 0x65, 0x28, 0x27, 0x6d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x27, 0x2c,
  0x20, 0x27, 0x70, 0x6f, 0x73, 0x74, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x66, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74, 0x74, 0x72, 0x69,
  0x62, 0x75, 0x74, 0x65, 0x28, 0x27, 0x61, 0x63, 0x74, 0x69, 0x6f, 0x6e,
  0x27, 0x2c, 0x20, 0x27, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x27, 0x29,
  0x3b, 0x0a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x61,
  0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x49, 0x64,
  0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e,
  0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e,
  0x74, 0x28, 0x27, 0x69, 0x6e, 0x70, 0x75, 0x74, 0x27, 0x29, 0x3b, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74,
  0x69, 0x6f, 0x6e, 0x49, 0x64, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74, 0x74,
  0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x27, 0x74, 0x79, 0x70, 0x65,
  0x27, 0x2c, 0x20, 0x27, 0x74, 0x65, 0x78, 0x74, 0x27, 0x29, 0x3b, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74,
  0x69, 0x6f, 0x6e, 0x49, 0x64, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74, 0x74,
  0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x27, 0x6e, 0x61, 0x6d, 0x65,
  0x27, 0x2c, 0x20, 0x27, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74,
  0x69, 0x6f, 0x6e, 0x49, 0x64, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e,
  0x49, 0x64, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62,
  0x75, 0x74, 0x65, 0x28, 0x27, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x27, 0x2c,
  0x20, 0x6a, 0x73, 0x6f, 0x6e, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63,
  0x61, 0x74, 0x69, 0x6f, 0x6e, 0x49, 0x64, 0x29, 0x3b, 0x0a, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x61, 0x63, 0x63, 0x65, 0x73,
  0x73, 0x54, 0x6f, 0x6b, 0x65, 0x6e, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63,
  0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65,
  0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x69, 0x6e, 0x70,
  0x75, 0x74, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x61, 0x63,
  0x63, 0x65, 0x73, 0x73, 0x54, 0x6f, 0x6b, 0x65, 0x6e, 0x2e, 0x73, 0x65,
  0x74, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x27,
  0x74, 0x79, 0x70, 0x65, 0x27, 0x2c, 0x20, 0x27, 0x74, 0x65, 0x78, 0x74,
  0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x61, 0x63, 0x63, 0x65,
  0x73, 0x73, 0x54, 0x6f, 0x6b, 0x65, 0x6e, 0x2e, 0x73, 0x65, 0x74, 0x41,
  0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x27, 0x6e, 0x61,
  0x6d, 0x65, 0x27, 0x2c, 0x20, 0x27, 0x61, 0x63, 0x63, 0x65, 0x73, 0x73,
  0x54, 0x6f, 0x6b, 0x65, 0x6e, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x61, 0x63, 0x63, 0x65, 0x73, 0x73, 0x54, 0x6f, 0x6b, 0x65, 0x6e,
  0x2e, 0x73, 0x65, 0x74, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74,
  0x65, 0x28, 0x27, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x27, 0x2c, 0x20, 0x6a,
  0x73, 0x6f, 0x6e, 0x2e, 0x61, 0x63, 0x63, 0x65, 0x73, 0x73, 0x54, 0x6f,
  0x6b, 0x65, 0x6e, 0x29, 0x3b, 0x0a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x76,
  0x61, 0x72, 0x20, 0x73, 0x6d, 0x61, 0x72, 0x74, 0x54, 0x68, 0x69, 0x6e,
  0x67, 0x73, 0x55, 0x72, 0x6c, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75,
  0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x45,
  0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x69, 0x6e, 0x70, 0x75,
  0x74, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x73, 0x6d, 0x61,
  0x72, 0x74, 0x54, 0x68, 0x69, 0x6e, 0x67, 0x73, 0x55, 0x72, 0x6c, 0x2e,
  0x73, 0x65, 0x74, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65,
  0x28, 0x27, 0x74, 0x79, 0x70, 0x65, 0x27, 0x2c, 0x20, 0x27, 0x74, 0x65,
  0x78, 0x74, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x73, 0x6d,
  0x61, 0x72, 0x74, 0x54, 0x68, 0x69, 0x6e, 0x67, 0x73, 0x55, 0x72, 0x6c,
  0x2e, 0x73, 0x65, 0x74, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74,
  0x65, 0x28, 0x27, 0x6e, 0x61, 0x6d, 0x65, 0x27, 0x2c, 0x20, 0x27, 0x73,
  0x6d, 0x61, 0x72, 0x74, 0x54, 0x68, 0x69, 0x6e, 0x67, 0x73, 0x55, 0x72,
  0x6c, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x73, 0x6d, 0x61,
  0x72, 0x74, 0x54, 0x68, 0x69, 0x6e, 0x67, 0x73, 0x55, 0x72, 0x6c, 0x2e,
  0x73, 0x65, 0x74, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65,
  0x28, 0x27, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x27, 0x2c, 0x20, 0x6a, 0x73,
  0x6f, 0x6e, 0x2e, 0x73, 0x6d, 0x61, 0x72, 0x74, 0x54, 0x68, 0x69, 0x6e,
  0x67, 0x73, 0x55, 0x72, 0x6c, 0x29, 0x3b, 0x0a, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x76, 0x61, 0x72, 0x20, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74,
  0x53, 0x74, 0x61, 0x74, 0x65, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75,
  0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x45,
  0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x73, 0x65, 0x6c, 0x65,
  0x63, 0x74, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x64, 0x65,
  0x66, 0x61, 0x75, 0x6c, 0x74, 0x53, 0x74, 0x61, 0x74, 0x65, 0x2e, 0x73,
  0x65, 0x74, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28,
  0x27, 0x6e, 0x61, 0x6d, 0x65, 0x27, 0x2c, 0x20, 0x27, 0x64, 0x65, 0x66,
  0x61, 0x75, 0x6c, 0x74, 0x53, 0x74, 0x61, 0x74, 0x65, 0x27, 0x29, 0x3b,
  0x0a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x6f, 0x70,
  0x74, 0x69, 0x6f, 0x6e, 0x31, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75,
  0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x45,
  0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x6f, 0x70, 0x74, 0x69,
  0x6f, 0x6e, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, 0x70,
  0x74, 0x69, 0x6f, 0x6e, 0x31, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74, 0x74,
  0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x27, 0x76, 0x61, 0x6c, 0x75,
  0x65, 0x27, 0x2c, 0x27, 0x30, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x31, 0x2e, 0x69, 0x6e, 0x6e,
  0x65, 0x72, 0x54, 0x65, 0x78, 0x74, 0x3d, 0x27, 0x6f, 0x66, 0x66, 0x27,
  0x3b, 0x0a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x6f,
  0x70, 0x74, 0x69, 0x6f, 0x6e, 0x32, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63,
  0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65,
  0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x6f, 0x70, 0x74,
  0x69, 0x6f, 0x6e, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f,
  0x70, 0x74, 0x69, 0x6f, 0x6e, 0x32, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74,
  0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x27, 0x76, 0x61, 0x6c,
  0x75, 0x65, 0x27, 0x2c, 0x27, 0x31, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x32, 0x2e, 0x69, 0x6e,
  0x6e, 0x65, 0x72, 0x54, 0x65, 0x78, 0x74, 0x3d, 0x27, 0x6f, 0x6e, 0x27,
  0x3b, 0x0a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x6f,
  0x70, 0x74, 0x69, 0x6f, 0x6e, 0x33, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63,
  0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65,
  0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x6f, 0x70, 0x74,
  0x69, 0x6f, 0x6e, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f,
  0x70, 0x74, 0x69, 0x6f, 0x6e, 0x33, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74,
  0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x27, 0x76, 0x61, 0x6c,
  0x75, 0x65, 0x27, 0x2c, 0x27, 0x32, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x33, 0x2e, 0x69, 0x6e,
  0x6e, 0x65, 0x72, 0x54, 0x65, 0x78, 0x74, 0x3d, 0x27, 0x6c, 0x61, 0x73,
  0x74, 0x27, 0x3b, 0x0a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x64, 0x65, 0x66,
  0x61, 0x75, 0x6c, 0x74, 0x53, 0x74, 0x61, 0x74, 0x65, 0x2e, 0x61, 0x64,
  0x64, 0x28, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x31, 0x29, 0x3b, 0x0a,
  0x20, 0x20, 0x20, 0x20, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x53,
  0x74, 0x61, 0x74, 0x65, 0x2e, 0x61, 0x64, 0x64, 0x28, 0x6f, 0x70, 0x74,
  0x69, 0x6f, 0x6e, 0x32, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x64,
  0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x53, 0x74, 0x61, 0x74, 0x65, 0x2e,
  0x61, 0x64, 0x64, 0x28, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x33, 0x29,
  0x3b, 0x0a, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x64, 0x65, 0x66, 0x61, 0x75,
  0x6c, 0x74, 0x53, 0x74, 0x61, 0x74, 0x65, 0x2e, 0x76, 0x61, 0x6c, 0x75,
  0x65, 0x20, 0x3d, 0x6a, 0x73, 0x6f, 0x6e, 0x2e, 0x64, 0x65, 0x66, 0x61,
  0x75, 0x6c, 0x74, 0x53, 0x74, 0x61, 0x74, 0x65, 0x3b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x73, 0x61, 0x76, 0x65, 0x20, 0x3d,
  0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72,
  0x65, 0x61, 0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28,
  0x27, 0x69, 0x6e, 0x70, 0x75, 0x74, 0x27, 0x29, 0x3b, 0x20, 0x2f, 0x2f,
  0x69, 0x6e, 0x70, 0x75, 0x74, 0x20, 0x65, 0x6c, 0x65, 0x6d, 0x65, 0x6e,
  0x74, 0x2c, 0x20, 0x53, 0x75, 0x62, 0x6d, 0x69, 0x74, 0x20, 0x62, 0x75,
  0x74, 0x74, 0x6f, 0x6e, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x73, 0x61, 0x76,
  0x65, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75,
  0x74, 0x65, 0x28, 0x27, 0x74, 0x79, 0x70, 0x65, 0x27, 0x2c, 0x20, 0x27,
  0x73, 0x75, 0x62, 0x6d, 0x69, 0x74, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x73, 0x61, 0x76, 0x65, 0x2e, 0x73, 0x65, 0x74, 0x41, 0x74,
  0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x28, 0x27, 0x76, 0x61, 0x6c,
  0x75, 0x65, 0x27, 0x2c, 0x20, 0x27, 0x53, 0x61, 0x76, 0x65, 0x27, 0x29,
  0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x70, 0x61,
  0x72, 0x61, 0x67, 0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65,
  0x6e, 0x74, 0x31, 0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65,
  0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x45, 0x6c, 0x65,
  0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x70, 0x27, 0x29, 0x3b, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x70, 0x61, 0x72, 0x61, 0x67, 0x72, 0x61, 0x70, 0x68,
  0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x31, 0x2e, 0x69, 0x6e, 0x6e,
  0x65, 0x72, 0x54, 0x65, 0x78, 0x74, 0x20, 0x3d, 0x20, 0x27, 0x53, 0x6d,
  0x61, 0x72, 0x74, 0x74, 0x68, 0x69, 0x6e, 0x67, 0x73, 0x20, 0x41, 0x70,
  0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x49, 0x64,
  0x3a, 0x20, 0x27, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x2e, 0x61, 0x70,
  0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28, 0x70, 0x61,
  0x72, 0x61, 0x67, 0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65,
  0x6e, 0x74, 0x31, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x61,
  0x72, 0x61, 0x67, 0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65,
  0x6e, 0x74, 0x31, 0x2e, 0x61, 0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68,
  0x69, 0x6c, 0x64, 0x28, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74,
  0x69, 0x6f, 0x6e, 0x49, 0x64, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20,
  0x66, 0x2e, 0x61, 0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c,
  0x64, 0x28, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63,
  0x72, 0x65, 0x61, 0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74,
  0x28, 0x27, 0x62, 0x72, 0x27, 0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x76, 0x61, 0x72, 0x20, 0x70, 0x61, 0x72, 0x61, 0x67, 0x72, 0x61,
  0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x32, 0x20, 0x3d,
  0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72,
  0x65, 0x61, 0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28,
  0x27, 0x70, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x61,
  0x72, 0x61, 0x67, 0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65,
  0x6e, 0x74, 0x32, 0x2e, 0x69, 0x6e, 0x6e, 0x65, 0x72, 0x54, 0x65, 0x78,
  0x74, 0x20, 0x3d, 0x20, 0x27, 0x53, 0x6d, 0x61, 0x72, 0x74, 0x74, 0x68,
  0x69, 0x6e, 0x67, 0x73, 0x20, 0x41, 0x63, 0x63, 0x65, 0x73, 0x73, 0x20,
  0x54, 0x6f, 0x6b, 0x65, 0x6e, 0x3a, 0x27, 0x0a, 0x20, 0x20, 0x20, 0x20,
  0x66, 0x2e, 0x61, 0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c,
  0x64, 0x28, 0x70, 0x61, 0x72, 0x61, 0x67, 0x72, 0x61, 0x70, 0x68, 0x45,
  0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x32, 0x29, 0x3b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x70, 0x61, 0x72, 0x61, 0x67, 0x72, 0x61, 0x70, 0x68, 0x45,
  0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x32, 0x2e, 0x61, 0x70, 0x70, 0x65,
  0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28, 0x61, 0x63, 0x63, 0x65,
  0x73, 0x73, 0x54, 0x6f, 0x6b, 0x65, 0x6e, 0x29, 0x3b, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x66, 0x2e, 0x61, 0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68,
  0x69, 0x6c, 0x64, 0x28, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74,
  0x2e, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65,
  0x6e, 0x74, 0x28, 0x27, 0x62, 0x72, 0x27, 0x29, 0x29, 0x3b, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x76, 0x61, 0x72, 0x20, 0x70, 0x61, 0x72, 0x61, 0x67,
  0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x33,
  0x20, 0x3d, 0x20, 0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e,
  0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e,
  0x74, 0x28, 0x27, 0x70, 0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20,
  0x70, 0x61, 0x72, 0x61, 0x67, 0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65,
  0x6d, 0x65, 0x6e, 0x74, 0x33, 0x2e, 0x69, 0x6e, 0x6e, 0x65, 0x72, 0x54,
  0x65, 0x78, 0x74, 0x20, 0x3d, 0x20, 0x27, 0x53, 0x6d, 0x61, 0x72, 0x74,
  0x74, 0x68, 0x69, 0x6e, 0x67, 0x73, 0x20, 0x55, 0x72, 0x6c, 0x3a, 0x27,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x2e, 0x61, 0x70, 0x70, 0x65, 0x6e,
  0x64, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28, 0x70, 0x61, 0x72, 0x61, 0x67,
  0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x33,
  0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x61, 0x72, 0x61, 0x67,
  0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x33,
  0x2e, 0x61, 0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c, 0x64,
  0x28, 0x73, 0x6d, 0x61, 0x72, 0x74, 0x54, 0x68, 0x69, 0x6e, 0x67, 0x73,
  0x55, 0x72, 0x6c, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x2e,
  0x61, 0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28,
  0x64, 0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65,
  0x61, 0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27,
  0x62, 0x72, 0x27, 0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x76,
  0x61, 0x72, 0x20, 0x70, 0x61, 0x72, 0x61, 0x67, 0x72, 0x61, 0x70, 0x68,
  0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x34, 0x20, 0x3d, 0x20, 0x64,
  0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61,
  0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x70,
  0x27, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x61, 0x72, 0x61,
  0x67, 0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74,
  0x34, 0x2e, 0x69, 0x6e, 0x6e, 0x65, 0x72, 0x54, 0x65, 0x78, 0x74, 0x20,
  0x3d, 0x20, 0x27, 0x52, 0x65, 0x6c, 0x61, 0x79, 0x20, 0x44, 0x65, 0x66,
  0x61, 0x75, 0x6c, 0x74, 0x20, 0x53, 0x74, 0x61, 0x74, 0x65, 0x3a, 0x27,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x2e, 0x61, 0x70, 0x70, 0x65, 0x6e,
  0x64, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28, 0x70, 0x61, 0x72, 0x61, 0x67,
  0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x34,
  0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x70, 0x61, 0x72, 0x61, 0x67,
  0x72, 0x61, 0x70, 0x68, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x34,
  0x2e, 0x61, 0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c, 0x64,
  0x28, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x53, 0x74, 0x61, 0x74,
  0x65, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x2e, 0x61, 0x70,
  0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28, 0x64, 0x6f,
  0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61, 0x74,
  0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x62, 0x72,
  0x27, 0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x2e, 0x61,
  0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28, 0x64,
  0x6f, 0x63, 0x75, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x63, 0x72, 0x65, 0x61,
  0x74, 0x65, 0x45, 0x6c, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x28, 0x27, 0x62,
  0x72, 0x27, 0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x2e,
  0x61, 0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28,
  0x73, 0x61, 0x76, 0x65, 0x29, 0x3b, 0x0a, 0x0a, 0x0a, 0x20, 0x20, 0x20,
  0x20, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x44, 0x69, 0x76, 0x2e, 0x61,
  0x70, 0x70, 0x65, 0x6e, 0x64, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x28, 0x66,
  0x29, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2f, 0x2f, 0x64, 0x65, 0x62,
  0x75, 0x67, 0x67, 0x65, 0x72, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2f,
  0x2f, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x44, 0x69, 0x76, 0x2e, 0x63,
  0x68, 0x0a, 0x20, 0x20, 0x7d, 0x29, 0x0a, 0x0a, 0x3c, 0x2f, 0x73, 0x63,
  0x72, 0x69, 0x70, 0x74, 0x3e, 0x0a, 0x3c, 0x2f, 0x68, 0x74, 0x6d, 0x6c,
  0x3e
};
unsigned int sonoffLine_html_len = 4309;


class TransportTraits
{
  public:
    virtual ~TransportTraits()
    {
    }

    virtual std::unique_ptr<WiFiClient> create()
    {
      return std::unique_ptr<WiFiClient>(new WiFiClient());
    }

    virtual bool verify(WiFiClient& client, const char* host)
    {
      (void)client;
      (void)host;
      return true;
    }
};

class TLSTraits : public TransportTraits
{
  public:
    TLSTraits()
    {
    }

    std::unique_ptr<WiFiClient> create() override
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored  "-Wdeprecated-declarations"
      return std::unique_ptr<WiFiClient>(new axTLS::WiFiClientSecure());
#pragma GCC diagnostic pop
    }

    bool verify(WiFiClient& client, const char* host) override
    {
      auto wcs = static_cast<axTLS::WiFiClientSecure&>(client);
      return true;
    }
};

class HTTPClient2 : public HTTPClient
{
  public:
    HTTPClient2()
    {
    }

    bool beginInternal2(String url, const char* expectedProtocol)
    {
      _transportTraits = TransportTraitsPtr(new TLSTraits());
      _port = 443;
      return beginInternal(url, expectedProtocol);
    }
};


ESP8266WebServer server ( 80 );

Sonoff sonoff;

const int led = LED;

typedef struct
{
  byte package;
  char smartThingsUrl[128];
  char applicationId[128];
  char accessToken[128];
  int  defaultState;
  bool  lastState;
  char signature[3];
} Configuration;

Configuration configuration {
  12,
  "",
  "",
  "",
  0,
  false,
  "OK"
};


String IpAddress2String(const IPAddress& ipAddress)
{
  String ip = String(ipAddress[0]) + String(".") + \
              String(ipAddress[1]) + String(".") + \
              String(ipAddress[2]) + String(".") + \
              String(ipAddress[3]);
  return ip  ;
}

void storeStruct(void *data_source, size_t size)
{
  EEPROM.begin(size * 2);
  for (size_t i = 0; i < size; i++)
  {
    char data = ((char *)data_source)[i];
    EEPROM.write(i, data);
  }
  EEPROM.commit();
}

void loadStruct(void *data_dest, size_t size)
{
  EEPROM.begin(size * 2);
  byte value = EEPROM.read(0);
  if (value = 12) {
    for (size_t i = 0; i < size; i++)
    {
      char data = EEPROM.read(i);
      ((char *)data_dest)[i] = data;
    }
  }

}

void saveSmartThingsSettings(String smartThingsUrl, String applicationId,   String accessToken, int defaultState) {
  smartThingsUrl.toCharArray(configuration.smartThingsUrl, smartThingsUrl.length() + 1);
  applicationId.toCharArray(configuration.applicationId, applicationId.length() + 1);
  accessToken.toCharArray(configuration.accessToken, accessToken.length() + 1);
  configuration.defaultState = defaultState;
  storeStruct(&configuration, sizeof(configuration));
}

void saveSmartThingsSettings(boolean lastState) {
  configuration.lastState = lastState;
  storeStruct(&configuration, sizeof(configuration));
}

void loadSmartThingsSettings() {
  Serial.println ( "start loading ..." );
  Configuration readConfiguration{
    12,
    "",
    "",
    "",
    0,
    false,
    "OK"
  };

  loadStruct(&readConfiguration, sizeof(readConfiguration));
  Serial.println ( " loaded" );
  if (String(readConfiguration.signature) == String("OK")) {
    Serial.println ( "Configuration is Valid: " + String(readConfiguration.signature) + " lastState: " + String(readConfiguration.lastState));
    configuration = readConfiguration;
  } else {
    Serial.println ( "Configuration inValid: " + String(readConfiguration.signature));
  }
}

void switchOn() {
  if (!sonoff.relay.isOn()) {
    sonoff.relay.on();
    digitalWrite ( led, 0 );
    saveSmartThingsSettings(true);
    if (String(configuration.applicationId) != String("") &&
        String(configuration.accessToken) != String("")) {
      HTTPClient2 http;
      String url = String(configuration.smartThingsUrl) + "/api/smartapps/installations/" + String(configuration.applicationId) + "/on?access_token=" + String(configuration.accessToken);
      Serial.println ( "Starting SmartThings Http On : " + url );
      http.beginInternal2(url, "https");
      http.addHeader("Content-Type", "application/json");
      http.POST("{\"ip\":\"" + IpAddress2String( WiFi.localIP()) + "\",\"mac\":\"" + String(WiFi.macAddress()) + "\"}");
      http.writeToStream(&Serial);
      http.end();
    }
  }
}

void switchOff() {
  if (sonoff.relay.isOn()) {
    sonoff.relay.off();
    digitalWrite ( led, 1 );
    saveSmartThingsSettings(false);
    if (String(configuration.applicationId) != String("") &&
        String(configuration.accessToken) != String("")) {
      HTTPClient2 http;
      String url = String(configuration.smartThingsUrl) + "/api/smartapps/installations/" + String(configuration.applicationId) + "/off?access_token=" + String(configuration.accessToken);
      Serial.println ( "Starting SmartThings Http off: " + url );
      http.beginInternal2(url, "https");
      http.addHeader("Content-Type", "application/json");
      http.POST("{\"ip\":\"" + IpAddress2String( WiFi.localIP()) + "\",\"mac\":\"" + String(WiFi.macAddress()) + "\"}");
      http.writeToStream(&Serial);
      http.end();
    }
  }
}

void smartthingsInit() {
  Serial.println ( "Starting SmartThings Init" );
  if (String(configuration.applicationId) != String("") &&
      String(configuration.accessToken) != String("")) {

    HTTPClient2 http;
    String url = String(configuration.smartThingsUrl) + "/api/smartapps/installations/" + String(configuration.applicationId) + "/" + "init?access_token=" + String(configuration.accessToken);
    Serial.println ( "Sending Http Get " + url );
    http.beginInternal2(url, "https");
    http.addHeader("Content-Type", "application/json");
    http.POST("{\"ip\":\""
              + IpAddress2String( WiFi.localIP())
              + "\",\"mac\":\""
              + String(WiFi.macAddress())
              + "\", \"relay\": \""
              + String(sonoff.relay.isOn() ? "on" : "off")
              + "\"}");
    http.writeToStream(&Serial);
    http.end();
  } else {
    Serial.println ( "SmartThings Init Skip" );
  }
}

void handleSettings () {
  String smartThingsUrl = server.arg("smartThingsUrl");
  String applicationId = server.arg("applicationId");
  String accessToken = server.arg("accessToken");
  String defaultStateString = server.arg("defaultState");
  saveSmartThingsSettings(smartThingsUrl, applicationId, accessToken, (defaultStateString == String("")) ? 0 : defaultStateString.toInt());
  smartthingsInit();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleToggle () {
  if (sonoff.relay.isOn()) {
    switchOff();
  } else {
    switchOn();
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOn () {
  switchOn();
  server.send ( 200, "application/json", "{ \"relay\": \"on\", \"ip\":\"" + IpAddress2String( WiFi.localIP()) + "\",\"mac\":\"" + String(WiFi.macAddress()) + "\" }" );
}

void handleOff () {
  switchOff();
  server.send ( 200, "application/json", "{ \"relay\": \"off\", \"ip\":\"" + IpAddress2String( WiFi.localIP()) + "\",\"mac\":\"" + String(WiFi.macAddress()) + "\" }" );
}

void handleState () {
  server.send ( 200, "application/json",
                "{ \"relay\": \""
                + String(sonoff.relay.isOn() ? "on" : "off")
                + "\",\"uptime\":" +
                String(millis()) +
                ", \"ssid\": \""
                + ssid +
                "\",\"ip\":\""
                + IpAddress2String( WiFi.localIP())
                + "\", \"mac\":\""
                + String(WiFi.macAddress())
                + "\", \"applicationId\":\""
                + String(configuration.applicationId)
                + "\", \"accessToken\":\""
                + String(configuration.accessToken)
                + "\", \"smartThingsUrl\":\""
                + String(configuration.smartThingsUrl)
                + "\", \"defaultState\":"
                + String(configuration.defaultState)
                + " }");
}

void handleinit () {
  smartthingsInit();
  handleState ();
}

void handleRoot() {
  server.send ( 200, "text/html", sonoffLine_html );
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

void setup ( void ) {
  pinMode ( led, OUTPUT );
  digitalWrite ( led, 1 );
  Serial.begin ( 9600 );
  WiFi.begin ( ssid, password );
  Serial.println ( String("Connecting Wifi ") + String(ssid) );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  loadSmartThingsSettings();

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println (IpAddress2String( WiFi.localIP()) );
  Serial.print ( "MAC address: " );
  Serial.println (WiFi.macAddress() );

  if ( MDNS.begin ( "Sonoff" ) ) {
    Serial.println ( "MDNS responder started" );
  }
  //smartthingsInit();

  server.on ( "/", handleRoot );
  server.on ( "/config", HTTP_POST, handleSettings );
  server.on ( "/toggle", HTTP_POST, handleToggle );
  server.on ( "/on", handleOn);
  server.on ( "/off", handleOff );
  server.on ( "/state", handleState);
  server.on ( "/health", handleinit);
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
  Serial.println ( "configuration.defaultState = " + String(configuration.defaultState));
  Serial.println ( "configuration.lastState = " + String(configuration.lastState) );

  if (configuration.defaultState == 1) {
    switchOn();
  } else if (configuration.defaultState == 2) {
    if (configuration.lastState) {
      switchOn();
    } else {
      switchOff();
    }
  } else {
    switchOff();
  }
}

void loop ( void ) {
  server.handleClient();
  sonoff.loop();

  if (sonoff.sw.getEvent() == SWITCH_EVENT_ON) {
    if (sonoff.relay.isOn()) {
      switchOff();
    }
    else {
      switchOn();
    }
  }
}