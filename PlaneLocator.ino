#include <GNSS.h>
static SpGnss Gnss;
void setup() {
  Serial2.begin(115200);
  sleep(3);
  Gnss.begin();
  Gnss.setInterval(20);
  Gnss.select(GPS);
  Gnss.start(COLD_START);
}
void loop()
{
  if (Gnss.waitUpdate(-1))
  {
    SpNavData NavData;
    Gnss.getNavData(&NavData);
    String AllText = "heartbeat:" + String(NavData.numSatellites);
    if (NavData.posDataExist != 0) AllText = "location: { lon:" + String(NavData.longitude, 5) + ", lat:" + String(NavData.latitude, 5) + " }, " + AllText;
    Serial2.print("{ " + AllText +  " }");
  }
}
