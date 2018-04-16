void sendEmail(String message)
{
  Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
  String subject = "SuYour IP adress is!";

  if (gsender->Subject(subject)->Send(Email, message)) {
    Serial.println("Message send.");
  } else {
    Serial.print("Error sending message: ");
    Serial.println(gsender->getError());
  }
}

