#ifndef GRADIENT_H
#define GRADIENT_H

#include <QGradient>
#include <QObject>
#include <QQmlEngine>

class LightingGradient : public QObject, public QLinearGradient
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QVariantList gradientStops READ gradientStops WRITE setGradientStops NOTIFY gradientStopsChanged FINAL)
    Q_PROPERTY(int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged FINAL)
    Q_PROPERTY(Preset preset READ preset WRITE setPreset NOTIFY presetChanged FINAL)

public:
    enum Preset {
        None = 0,
        WarmFlame = 1,
        NightFade = 2,
        SpringWarmth = 3,
        JuicyPeach = 4,
        YoungPassion = 5,
        LadyLips = 6,
        SunnyMorning = 7,
        RainyAshville = 8,
        FrozenDreams = 9,
        WinterNeva = 10,
        DustyGrass = 11,
        TemptingAzure = 12,
        HeavyRain = 13,
        AmyCrisp = 14,
        MeanFruit = 15,
        DeepBlue = 16,
        RipeMalinka = 17,
        CloudyKnoxville = 18,
        MalibuBeach = 19,
        NewLife = 20,
        TrueSunset = 21,
        MorpheusDen = 22,
        RareWind = 23,
        NearMoon = 24,
        WildApple = 25,
        SaintPetersburg = 26,
        PlumPlate = 28,
        EverlastingSky = 29,
        HappyFisher = 30,
        Blessing = 31,
        SharpeyeEagle = 32,
        LadogaBottom = 33,
        LemonGate = 34,
        ItmeoBranding = 35,
        ZeusMiracle = 36,
        OldHat = 37,
        StarWine = 38,
        HappyAcid = 41,
        AwesomePine = 42,
        NewYork = 43,
        ShyRainbow = 44,
        MixedHopes = 46,
        FlyHigh = 47,
        StrongBliss = 48,
        FreshMilk = 49,
        SnowAgain = 50,
        FebruaryInk = 51,
        KindSteel = 52,
        SoftGrass = 53,
        GrownEarly = 54,
        SharpBlues = 55,
        ShadyWater = 56,
        DirtyBeauty = 57,
        GreatWhale = 58,
        TeenNotebook = 59,
        PoliteRumors = 60,
        SweetPeriod = 61,
        WideMatrix = 62,
        SoftCherish = 63,
        RedSalvation = 64,
        BurningSpring = 65,
        NightParty = 66,
        SkyGlider = 67,
        HeavenPeach = 68,
        PurpleDivision = 69,
        AquaSplash = 70,
        SpikyNaga = 72,
        LoveKiss = 73,
        CleanMirror = 75,
        PremiumDark = 76,
        ColdEvening = 77,
        CochitiLake = 78,
        SummerGames = 79,
        PassionateBed = 80,
        MountainRock = 81,
        DesertHump = 82,
        JungleDay = 83,
        PhoenixStart = 84,
        OctoberSilence = 85,
        FarawayRiver = 86,
        AlchemistLab = 87,
        OverSun = 88,
        PremiumWhite = 89,
        MarsParty = 90,
        EternalConstance = 91,
        JapanBlush = 92,
        SmilingRain = 93,
        CloudyApple = 94,
        BigMango = 95,
        HealthyWater = 96,
        AmourAmour = 97,
        RiskyConcrete = 98,
        StrongStick = 99,
        ViciousStance = 100,
        PaloAlto = 101,
        HappyMemories = 102,
        MidnightBloom = 103,
        Crystalline = 104,
        PartyBliss = 106,
        ConfidentCloud = 107,
        LeCocktail = 108,
        RiverCity = 109,
        FrozenBerry = 110,
        ChildCare = 112,
        FlyingLemon = 113,
        NewRetrowave = 114,
        HiddenJaguar = 115,
        AboveTheSky = 116,
        Nega = 117,
        DenseWater = 118,
        Seashore = 120,
        MarbleWall = 121,
        CheerfulCaramel = 122,
        NightSky = 123,
        MagicLake = 124,
        YoungGrass = 125,
        ColorfulPeach = 126,
        GentleCare = 127,
        PlumBath = 128,
        HappyUnicorn = 129,
        AfricanField = 131,
        SolidStone = 132,
        OrangeJuice = 133,
        GlassWater = 134,
        NorthMiracle = 136,
        FruitBlend = 137,
        MillenniumPine = 138,
        HighFlight = 139,
        MoleHall = 140,
        SpaceShift = 142,
        ForestInei = 143,
        RoyalGarden = 144,
        RichMetal = 145,
        JuicyCake = 146,
        SmartIndigo = 147,
        SandStrike = 148,
        NorseBeauty = 149,
        AquaGuidance = 150,
        SunVeggie = 151,
        SeaLord = 152,
        BlackSea = 153,
        GrassShampoo = 154,
        LandingAircraft = 155,
        WitchDance = 156,
        SleeplessNight = 157,
        AngelCare = 158,
        CrystalRiver = 159,
        SoftLipstick = 160,
        SaltMountain = 161,
        PerfectWhite = 162,
        FreshOasis = 163,
        StrictNovember = 164,
        MorningSalad = 165,
        DeepRelief = 166,
        SeaStrike = 167,
        NightCall = 168,
        SupremeSky = 169,
        LightBlue = 170,
        MindCrawl = 171,
        LilyMeadow = 172,
        SugarLollipop = 173,
        SweetDessert = 174,
        MagicRay = 175,
        TeenParty = 176,
        FrozenHeat = 177,
        GagarinView = 178,
        FabledSunset = 179,
        PerfectBlue = 180,

        NumPresets
    };
    Q_ENUM(Preset);
    explicit LightingGradient(QObject *parent = nullptr);

    QColor colorAt(double value);

    void setResolution(int newResolution);
    int resolution() const;

    QVariantList gradientStops() const;
    void setGradientStops(const QVariantList &newGradientStops);

    Preset preset() const;
    void setPreset(const Preset &newPreset);

public slots:
    QString presetName(int preset);
    void updateGradientStops();
    void updateColors();

signals:
    void gradientStopsChanged();
    void resolutionChanged();
    void presetChanged();

protected:
    void _setStops(const QGradientStops &sstops);

private:
    int _resolution;
    Preset _preset;
    QVector<QColor> _colors;
    QVariantList _gradientStops;
};

#endif // GRADIENT_H
