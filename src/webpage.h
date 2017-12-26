#pragma once
#include <Arduino.h>

/*
The result of convert the index.html and config.html with this very useful tool:
http://tomeko.net/online_tools/cpp_text_escape.php?lang=en
*/


String style = "  <style>\n"
"    body{\n"
"        background-color:#b2dfdb;\n"
"        }\n"
"    a{\n"
"        margin:.8em;\n"
"        display:block;\n"
"        text-align: center;\n"
"        padding: .8em 3em;\n"
"        background: green;\n"
"        width: auto;\n"
"        padding: .8em 3em;\n"
"        color:white;\n"
"        text-decoration:none;\n"
"        border-radius: 1em;\n"
"    }\n"
"\n"
"    #footer {\n"
"        position:absolute;\n"
"        bottom:0;\n"
"        width:98%;\n"
"        height:60px;\n"
"        box-sizing: border-box;\n"
"    }\n"
"  </style>\n";

//

String page_index = "<html>\n"
"  <head>\n"
"      <meta charset='utf-8'>\n"
"  </head>\n"
+ style +
"  <body>\n"
"      <a href=\"web_on\">Turn On</a>\n"
"      <a href=\"web_off\">Off</a>\n";

String html_disableswitch = "      <a href=\"web_disableswitch\">Disable Fisical</a>\n";
String html_enableswitch = "<a href=\"web_enableswitch\">Enable Fisical</a>\n";
String html_light_on = "      <center><h1>💡</h1></center>\n";
String html_light_off = "      <center><h1>🕯️</h1></center>\n";


String page_index_end = "      <a id=\"footer\" href=\"config\">Configuration</a>\n"
"  </body>\n"
"</html>\n";

String page_off_switch_off = page_index + html_enableswitch + html_light_off + page_index_end;
String page_off_switch_on = page_index + html_disableswitch + html_light_off + page_index_end;
String page_on_switch_off = page_index + html_enableswitch + html_light_on + page_index_end;
String page_on_switch_on = page_index + html_disableswitch + html_light_on + page_index_end;


String page_config = "<html>\n"
"    <style>\n"
"        body{\n"
"            background-color:#b2dfdb;\n"
"            }\n"
"        a{\n"
"            margin:.8em;\n"
"            display:block;\n"
"            text-align: center;\n"
"            padding: .8em 3em;\n"
"            background: green;\n"
"            width: auto;\n"
"            padding: .8em 3em;\n"
"            color:white;\n"
"            text-decoration:none;\n"
"            border-radius: 1em;\n"
"        }\n"
"\n"
"        input {\n"
"                 margin: 0.8em 00.8em;\n"
"                 display:block;\n"
"                 padding: .8em 1em;\n"
"                 background: white;\n"
"                 width: 99%;\n"
"                 /*! padding: .8em 3em; */\n"
"                 box-sizing : border-box;\n"
"                 text-decoration:none;\n"
"                 border-radius: 0.2em;\n"
"             }\n"
"\n"
"        div {\n"
"           margin: 0.8em;\n"
"        }\n"
"\n"
"        form {\n"
"            border-style: solid;\n"
"            border-width: 1px;\n"
"            border-radius: 0.2em;\n"
"        }\n"
"\n"
"        .submit {\n"
"            background: green;\n"
"            color: white;\n"
"        }\n"
"\n"
"        .warning {\n"
"            text-align: center;\n"
"            color: red;\n"
"        }\n"
"    </style>\n"
"    <head><meta charset='utf-8'></head>\n"
"    <body>\n"
"        <form action=\"/\" method=\"post\">\n"
"            <div>mDNS name</div>\n"
"            <input type=\"text\" name=\"mdns\" length=\"30\" />\n"
"            <input type=\"submit\" class=\"submit\" value=\"Save\" />\n"
"        </form>\n"
"        <form action=\"/\" method=\"post\">\n"
"            <div>SSID</div>\n"
"            <input type=\"text\" name=\"SSID\" length=\"10\" />\n"
"            <div>PASSWORD</div>\n"
"            <input type=\"password\" name=\"PSK\" lenth=\"10\" />\n"
"            <div class=\"warning\">The smart light will connect to the specified network when the power is turned on again.</div>\n"
"            <input type=\"submit\" class=\"submit\" value=\"Save\" />\n"
"            <a href=\\return\\>Return</a>\n"
"        </form>\n"
"    </body>\n"
"</html>\n";
