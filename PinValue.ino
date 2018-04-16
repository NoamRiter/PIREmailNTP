void PinValue()
{
  String data = server.arg("plain");
  yield();
  StaticJsonBuffer<200> jBuffer;
  JsonObject& jObject = jBuffer.parseObject(data);
  int pinNumber = jObject["pin"];
  int value = jObject["value"];
  yield();
  Serial.print("pivalue");
  Serial.println(pinNumber);
  switch ((int)pinNumber) {
    case 1:

      Serial.print("try to send email");
      yield();
      emailTobeSend[0] = 'B';
      saveJson();
      yield();
      sendEmail("Alarm have been triggered");
      yield();
      emailTobeSend[0] = 'A';
      saveJson();
      yield();
      break;

    case 2:


      break;

    default:
      //do nothing!
      break;
  }

}
