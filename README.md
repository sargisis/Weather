# 🌦️ Weather-App  

> **A modern Qt-based weather dashboard** that combines real-time forecasts, elegant UI, and fast performance — built with 💙 in C++.

---

## 🧭 Overview  

**Weather-App** is a desktop application built with **Qt (C++ Framework)**.  
It provides live weather data, intuitive search, and detailed forecasts for any city or country around the world.  

🧠 Designed for clarity, speed, and expansion — this app is part of my **journey into advanced Qt development** and UI/UX optimization.  

---

## ✨ Features  

⚡ **Live Weather Updates** – real-time temperature, humidity, and conditions  
🌍 **Global City Search** – type any city name and get instant results  
📅 **Forecast View** – see weather trends for the next few days  
📱 **Modern UI** – custom Qt Widgets with smooth layout and visuals  
🌐 **API Integration** – uses `QtNetwork` for fetching data from weather APIs  

---

## 🛠️ Tech Stack  

| Layer | Technology |
|--------|-------------|
| 🧩 **Language** | C++ |
| 🖼️ **Framework** | Qt (Widgets, Core, GUI, Network) |
| ☁️ **API Layer** | OpenWeatherMap / WeatherAPI (via QtNetwork) |
| 🎨 **UI Design** | Qt Designer + Custom Stylesheets |
| ⚙️ **Build System** | qmake / CMake |

---

## 🚀 How to Run  

1. Install **Qt** (recommended: Qt 6.x) and **Qt Creator** IDE.  
2. Clone the project:
   ``bash
   git clone https://github.com/sargisis/Weather_App.git
3. Open the .pro file or CMakeLists.txt in Qt Creator.
4. Make sure the Qt Network module is enabled:
   QT += core gui widgets network
5. Build ➜ Run ➜ Enjoy 🌤️
