//
//  ColorPickerApp.cpp
//  ColorPicker
//
//  Created by Jean-Pierre Mouilleseaux on 8 Jul 2015.
//  Copyright 2015 Chorded Constructions. All rights reserved.
//

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/Log.h"

#include "Cinder-Serial.h"

#include "boost/format.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace Cinder::Serial;

// http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
// http://wolfr.am/5NzJsMbR
inline float easeInOutBrething(float t) {
    return (expf(sinf(t * M_PI)) - 1/M_E) * 1/(M_E-(1/M_E));
}

class ColorPickerApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;

private:
    enum class ColorMode {RedBlueFade = 0, Breath, SOS, Patriot};

    void cycleColorMode();
    void sendColor();

    SerialDeviceRef mDevice;
    TimelineRef mTimeline;
    Anim<Colorf> mColor;
    ColorMode mColorMode;
};

void ColorPickerApp::setup() {
    // iterate through ports
    auto ports = SerialPort::getPorts();
    for (auto port : ports) {
        console() << "SERIAL DEVICE" << endl;
        console() << "\tNAME: " << port->getName() << endl;
        console() << "\tDESCRIPTION: " << port->getDescription() << endl;
        console() << "\tHARDWARE IDENTIFIER: " << port->getHardwareIdentifier() << endl;
    }

    // grab a port and create a device
    if (!ports.empty()) {
        SerialPortRef port = SerialPort::findPortByNameMatching(std::regex("\\/dev\\/cu\\.usbmodem.*"));
        if (!port) {
            port = ports.back();
        }

        try {
            mDevice = SerialDevice::create(port, 115200);
        } catch (serial::IOException& e) {
            CI_LOG_EXCEPTION("failed to create serial device", e);
            quit();
        }

        // NB: device is opened on creation
    }

    mTimeline = Timeline::create();
    mTimeline->setDefaultAutoRemove(false);
    timeline().add(mTimeline);

    mColor = Color::black();
    mColorMode = ColorMode::RedBlueFade;
}

void ColorPickerApp::update() {
    if (!mDevice) {
        return;
    }

    sendColor();

    uint8_t buffer[3];
    while (mDevice->getNumberOfAvailableBytes() > 0) {
        size_t size = mDevice->readBytes(buffer, 3);
        // scan buffer for message when appropraitely large
        if (size == 3) {
            if (buffer[1] == '\r' && buffer[2] == '\n') {
                // 1 when the button goes HIGH
                if (buffer[0] == 1) {
                    cycleColorMode();
                }
            }
        }
    }
}

void ColorPickerApp::draw() {
	gl::clear(mColor());
}

#pragma mark - PRIVATE
void ColorPickerApp::cycleColorMode() {
    float timeUnit = 0.2f;

    switch (mColorMode) {
        case ColorMode::RedBlueFade:
            mTimeline->appendTo(&mColor, Color::black(), 7.0f * timeUnit);
            mTimeline->appendTo(&mColor, Colorf(1.0f, 0.0f, 0.0f), Colorf(0.0f, 0.0f, 1.0f), 14.0f * timeUnit, easeInOutQuad).pingPong(true);
            break;
        case ColorMode::Breath:
            mTimeline->appendTo(&mColor, Color::black(), 7.0f * timeUnit);
            mTimeline->appendTo(&mColor, Colorf(0.0f, 0.0f, 0.0f), Colorf(1.0f, 0.0f, 0.0f), 14.0f * timeUnit, easeInOutBrething).loop();
            break;
        case ColorMode::SOS:
            mTimeline->appendTo(&mColor, Color::black(), 7.0f * timeUnit);

            // · · · — — · · ·
            mTimeline->appendTo(&mColor, Color::white(), Color::white(), timeUnit);
            mTimeline->appendTo(&mColor, Color::black(), Color::black(), timeUnit);
            mTimeline->appendTo(&mColor, Color::white(), Color::white(), timeUnit);
            mTimeline->appendTo(&mColor, Color::black(), Color::black(), timeUnit);
            mTimeline->appendTo(&mColor, Color::white(), Color::white(), timeUnit);

            mTimeline->appendTo(&mColor, Color::black(), Color::black(), 3.0f * timeUnit);

            mTimeline->appendTo(&mColor, Color::white(), Color::white(), 3.0f * timeUnit);
            mTimeline->appendTo(&mColor, Color::black(), Color::black(), timeUnit);
            mTimeline->appendTo(&mColor, Color::white(), Color::white(), 3.0f * timeUnit);
            mTimeline->appendTo(&mColor, Color::black(), Color::black(), timeUnit);
            mTimeline->appendTo(&mColor, Color::white(), Color::white(), 3.0f * timeUnit);

            mTimeline->appendTo(&mColor, Color::black(), Color::black(), 3.0f * timeUnit);

            mTimeline->appendTo(&mColor, Color::white(), Color::white(), timeUnit);
            mTimeline->appendTo(&mColor, Color::black(), Color::black(), timeUnit);
            mTimeline->appendTo(&mColor, Color::white(), Color::white(), timeUnit);
            mTimeline->appendTo(&mColor, Color::black(), Color::black(), timeUnit);
            mTimeline->appendTo(&mColor, Color::white(), Color::white(), timeUnit);

            mTimeline->appendTo(&mColor, Color::black(), Color::black(), 7.0f * timeUnit);
            break;
        case ColorMode::Patriot:
            mTimeline->appendTo(&mColor, Color::black(), 7.0f * timeUnit);

            // 🇺🇸
            mTimeline->appendTo(&mColor, Colorf(1.0f, 0.0f, 0.0f), Colorf(1.0f, 0.0f, 0.0f), 4.0f * timeUnit);
            mTimeline->appendTo(&mColor, Color::white(), Color::white(), 4.0f * timeUnit);
            mTimeline->appendTo(&mColor, Colorf(0.0f, 0.0f, 1.0f), Colorf(0.0f, 0.0f, 1.0f), 4.0f * timeUnit);

            mTimeline->appendTo(&mColor, Color::black(), Color::black(), 7.0f * timeUnit);
            break;
    }

    mColorMode = static_cast<ColorMode>(static_cast<int>(mColorMode)+1);
    if (mColorMode > ColorMode::Patriot) {
        mColorMode = ColorMode::RedBlueFade;
    }
}

void ColorPickerApp::sendColor() {
    if (!mDevice) {
        return;
    }

    uint8_t r = static_cast<uint8_t>(round(math<float>::clamp(mColor().r, 0.0f, 1.0f) * 255.0f));
    uint8_t g = static_cast<uint8_t>(round(math<float>::clamp(mColor().g, 0.0f, 1.0f) * 255.0f));
    uint8_t b = static_cast<uint8_t>(round(math<float>::clamp(mColor().b, 0.0f, 1.0f) * 255.0f));

    // outgoing message is 5 bytes
    uint8_t buffer[5] = {
        r, g, b,
        '\r', '\n',
    };

    size_t size = mDevice->writeBytes(buffer, 5);
    if (size != 5) {
        CI_LOG_E(str(boost::format("only sent %1% bytes, should have sent 5!") % size));
        return;
    }
}

CINDER_APP(ColorPickerApp, RendererGl)
