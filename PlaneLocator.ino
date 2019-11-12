#include <GNSS.h>
static SpGnss Gnss;
void setup() {
  Serial2.begin(115200);
  sleep(3);
  Gnss.begin();
  Gnss.setInterval(10);
  Gnss.select(GPS);
  Gnss.start(COLD_START);
}
void loop()
{
  if (Gnss.waitUpdate(-1))
  {
    SpNavData NavData;
    Gnss.getNavData(&NavData);
    if (NavData.posDataExist != 0) Serial2.print("{ location: { lon:" + String(NavData.longitude, 5) + ", lat:" + String(NavData.latitude, 5) + " } }");
  }
}
