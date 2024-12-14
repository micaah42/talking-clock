#ifndef PIXEL_H
#define PIXEL_H

#include <QColor>
#include <QObject>

typedef uint32_t ws2811_led_t;

class Pixel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)

public:
    explicit Pixel(ws2811_led_t &ws2811_led, QObject *parent = nullptr);

    int white() const;
    // void setWhite(int v);

    int green() const;
    // void setGreen(int v);

    int blue() const;
    // void setBlue(int v);

    int red() const;
    // void setRed(int v);

    const ws2811_led_t &ws2811_led() const;

    QColor color() const;
    void setColor(const QColor &newColor);

signals:
    void colorChanged();

protected:
    void _setWhite(int v);
    void _setGreen(int v);
    void _setBlue(int v);
    void _setRed(int v);

private:
    ws2811_led_t &_ws2811_led;
    QColor _color;
};

#endif // PIXEL_H
