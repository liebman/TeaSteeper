/*
 * MIT License
 *
 * Copyright (c) 2021 Christopher B. Liebman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "ConfigPortal.h"
#include <functional>
#include "Log.h"
#include "AsyncElegantOTA.h"

static const char* TAG = "ConfigPortal";
static const uint16_t DNS_PORT = 53;

ConfigPortal::ConfigPortal()
: _web(80),
  _dns(),
  _active(false)
{
}

ConfigPortal::~ConfigPortal()
{
    dlog.info(TAG, "::~ConfigPortal");
}

void ConfigPortal::startPortal(const char* name)
{
    dlog.info(TAG, "::startPortal");

    String deviceName(name);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(deviceName.c_str());
    dlog.info(TAG, "started softAP with IP %s", WiFi.softAPIP().toString());
    _dns.setErrorReplyCode(DNSReplyCode::NoError);
    if (!_dns.start(DNS_PORT, "*", WiFi.softAPIP()))
    {
        dlog.error(TAG, "::startPortal failed to start DNS server on %s:%u", WiFi.softAPIP().toString(), DNS_PORT);
        return;
    }
    dlog.info(TAG, "::startPortal started DNS server");
    _web.onNotFound([&](AsyncWebServerRequest *request){
        const char* method = request->methodToString();
        dlog.warning(TAG, "serveing not found %s %s", method, request->url().c_str());
        AsyncWebServerResponse * response = request->beginResponse(307);
        response->addHeader("Location", "http://192.168.4.1/");
        request->send(response);
    });

    _web.on("/", HTTP_GET, [&](AsyncWebServerRequest *request){
        dlog.info(TAG, "serve / (redirect to /update)");
        request->redirect("http://192.168.4.1/update");
    });

    AsyncElegantOTA.begin(&_web);

    dlog.info(TAG, "::startPortal starting WEB server free=%lu", ESP.getFreeHeap());
    _web.begin();
    dlog.info(TAG, "::startPortal start WEB server free=%lu", ESP.getFreeHeap());
    _active = true;
}

void ConfigPortal::poll()
{
    if (_active)
    {
        _dns.processNextRequest();
    }
}
