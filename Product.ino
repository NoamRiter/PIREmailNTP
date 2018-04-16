void Product() {
  javascriptProduct();
  getHeader();

  webpage += "<div class=\"container\">\n";
webpage += "<br />\n";
webpage += "<br />\n";

webpage += "<div class=\"col-sm-2 col-md-2\">\n";
webpage += "</div>\n";

  webpage += "<div class=\"col-sm-2 col-md-2\">\n";
  
  if (isEmail)
  {
    webpage += "<button id=\"1\" type=\"button\" class=\"btn btn-success\" onClick=\"sendPWM(1,1)\">01</button>\n";
  }
  else
  {
    webpage += "<button id=\"1\" type=\"button\" class=\"btn btn-danger\" onClick=\"sendPWM(1,1)\">01</button>\n";
  }
  webpage += "</div>\n";


/*
  webpage += " <div class=\"col-sm-2 col-md-2\">\n";
  if (pin2Status)
  {
    webpage += "<button id=\"2\" type=\"button\" class=\"btn btn-success\" onClick=\"sendPWM(2,1)\">02</button>\n";
  }
  else
  {
    webpage += "<button id=\"2\" type=\"button\" class=\"btn btn-danger\" onClick=\"sendPWM(2,1)\">02</button>\n";
  }
  webpage += "</div>\n";

  webpage += " <div class=\"col-sm-2 col-md-2\">\n";
  if (pin3Status)
  {
    webpage += "<button id=\"3\" type=\"button\" class=\"btn btn-success\" onClick=\"sendPWM(3,1)\">03</button>\n";
  }
  else
  {
    webpage += "<button id=\"3\" type=\"button\" class=\"btn btn-danger\" onClick=\"sendPWM(3,1)\">03</button>\n";
  }
  webpage += "</div>\n";

  webpage += " <div class=\"col-sm-2 col-md-2\">\n";
  if (pin4Status)
  {
    webpage += "<button id=\"4\" type=\"button\" class=\"btn btn-success\" onClick=\"sendPWM(4,1)\">04</button>\n";
  }
  else
  {
    webpage += "<button id=\"4\" type=\"button\" class=\"btn btn-danger\" onClick=\"sendPWM(4,1)\">04</button>\n";
  }
  webpage += "</div>\n";
  */
  webpage += "<div class=\"col-sm-2 col-md-2\">\n";
webpage += "</div>\n";

  webpage += "</div>\n";

  webpage += "<br />\n";
  webpage += "<br />\n";
  webpage += "<br />\n";
  webpage += "<br />\n";
  webpage += "<br />\n";

 // webpage += "<div class = \"container\">\n";
 // webpage += "<button type=\"button\" class=\"btn btn-success\" onClick=\"SendCom()\">Restart wifiManager</button>\n"; //Need To reset modul manual just nu
 // webpage += "</div>\n";

  getFooter();

  server.send(200, "text/html", webpage);
}

void javascriptProduct() { // here you can add javascript to this page
  javaScript = "<script type=\"text/javascript\">\n";
  javaScript += "function SendCom()\n";
  javaScript += "{\n";
  javaScript += "window.open(\"http://" + IpAddress + "/Password\", \"PassWord Manager \", 'width=250,height=250')\n";
  javaScript += "}\n";

  javaScript += "function sendPWM(Pin,Value){\n";
  javaScript += "if(document.getElementById(Pin).classList.contains(\"btn-success\"))\n";
  javaScript += "{\n";
  javaScript += "document.getElementById(Pin).classList.remove(\"btn-success\");\n";
  javaScript += "document.getElementById(Pin).classList.add(\"btn-danger\");\n";
  javaScript += "}\n";
  javaScript += "else\n";
  javaScript += "{\n";
  javaScript += "document.getElementById(Pin).classList.remove(\"btn-danger\");\n";
  javaScript += "document.getElementById(Pin).classList.add(\"btn-success\");\n";
  javaScript += "}\n";
  javaScript += "spChange = \"spPWM\" + Pin;\n";
  javaScript += "req = new XMLHttpRequest();\n";
  javaScript += "url =\"http://" + IpAddress + "/PinValue\";\n";
  javaScript += "var data = {pin: Pin, value: Value};\n";
  javaScript += "req.open(\"POST\", url);\n";
  javaScript += "req.send(JSON.stringify(data));\n";
  javaScript += "}\n";
  javaScript += "</script>\n";
}
