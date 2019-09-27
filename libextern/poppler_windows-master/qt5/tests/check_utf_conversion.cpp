#include <QtCore/QScopedPointer>
#include <QtTest/QtTest>

#include <cstring>
#include "UTF.h"

class TestUTFConversion : public QObject
{
    Q_OBJECT
public:
    TestUTFConversion(QObject *parent = nullptr) : QObject(parent) { }
private slots:
    void testUTF_data();
    void testUTF();
};

static bool compare(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

static bool compare(const uint16_t *a, const uint16_t *b)
{
    while (*a && *b) {
	if (*a++ != *b++)
	    return false;
    }
    return *a == *b;
}

void TestUTFConversion::testUTF_data()
{
   QTest::addColumn<QString>("s");

   QTest::newRow("<empty>") << QString(QLatin1String(""));
   QTest::newRow("a") << QStringLiteral("a");
   QTest::newRow("abc") << QStringLiteral("abc");
   QTest::newRow("Latin") << QStringLiteral("Vitrum edere possum; mihi non nocet");
   QTest::newRow("Greek") << QStringLiteral("Μπορώ να φάω σπασμένα γυαλιά χωρίς να πάθω τίποτα");
   QTest::newRow("Icelandic") << QStringLiteral("Ég get etið gler án þess að meiða mig");
   QTest::newRow("Russian") << QStringLiteral("Я могу есть стекло, оно мне не вредит.");
   QTest::newRow("Sanskrit") << QStringLiteral("काचं शक्नोम्यत्तुम् । नोपहिनस्ति माम् ॥");
   QTest::newRow("Arabic") << QStringLiteral("أنا قادر على أكل الزجاج و هذا لا يؤلمني");
   QTest::newRow("Chinese") << QStringLiteral("我能吞下玻璃而不伤身体。");
   QTest::newRow("Thai") << QStringLiteral("ฉันกินกระจกได้ แต่มันไม่ทำให้ฉันเจ็บ");
   QTest::newRow("non BMP") << QStringLiteral("𝓹𝓸𝓹𝓹𝓵𝓮𝓻");
 }

void TestUTFConversion::testUTF()
{
    char utf8Buf[1000];
    char *utf8String;
    uint16_t utf16Buf[1000];
    uint16_t *utf16String;
    int len;

    QFETCH(QString, s);
    char *str = strdup(s.toUtf8().constData());

    // UTF-8 to UTF-16

    len = utf8CountUtf16CodeUnits(str);
    QCOMPARE( len, s.size() ); // QString size() returns number of code units, not code points
    Q_ASSERT( len < (int)sizeof(utf16Buf) ); // if this fails, make utf16Buf larger

    len = utf8ToUtf16(str, utf16Buf);
    QVERIFY( compare(utf16Buf, s.utf16()) );
    QCOMPARE( len, s.size() );

    utf16String = utf8ToUtf16(str);
    QVERIFY( compare(utf16String, s.utf16()) );
    free (utf16String);

    // UTF-16 to UTF-8

    len = utf16CountUtf8Bytes(s.utf16());
    QCOMPARE( len, (int)strlen(str) );
    Q_ASSERT( len < (int)sizeof(utf8Buf) ); // if this fails, make utf8Buf larger

    len = utf16ToUtf8(s.utf16(), utf8Buf);
    QVERIFY( compare(utf8Buf, str) );
    QCOMPARE( len, (int)strlen(str) );

    utf8String = utf16ToUtf8(s.utf16() );
    QVERIFY( compare(utf8String, str) );
    free (utf8String);

    free(str);
}

QTEST_GUILESS_MAIN(TestUTFConversion)
#include "check_utf_conversion.moc"
