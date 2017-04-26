wifi.setmode(wifi.STATION)
wifi.sta.config("Hassani","HassaniLab")
print(wifi.sta.getip())

right=1
left=6
-- b=4

gpio.mode(right,gpio.OUTPUT)
gpio.mode(left,gpio.OUTPUT)
-- gpio.mode(b,gpio.OUTPUT)
pwm.setup(right,100,0)
pwm.setup(left,100,0)
-- pwm.setup(b,1000,1023)
pwm.start(right)
pwm.start(left)
-- pwm.start(b)

function off()
pwm.setduty(right,0)
pwm.setduty(left,0)
end

function forward()
pwm.setduty(right,180)
pwm.setduty(left,120)
end

function backward()
pwm.setduty(right,120)
pwm.setduty(left,180)
end

function clockwise()
pwm.setduty(right,120)
pwm.setduty(left,120)
end

function counterclockwise()
pwm.setduty(right,180)
pwm.setduty(left,180)
end

srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
    conn:on("receive", function(client,request)

local buf = "";
        buf = buf.."HTTP/1.1 200 OK\n\n"
local _, _, method, path, vars = string.find(request, "([A-Z]+) (.+)?(.+) HTTP");
if(method == nil)then
            _, _, method, path = string.find(request, "([A-Z]+) (.+) HTTP");
end

local _GET = {}
if (vars ~= nil)then
for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do
                _GET[k] = v
end
end
        buf = buf.."<!DOCTYPE HTML>"
        buf = buf.."<html><head>";
        buf = buf.."<meta charset=\"utf-8\">";
        buf = buf.."<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
        buf = buf.."<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
        buf = buf.."<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
        buf = buf.."<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">";
        buf = buf.."</head><div class=\"container\">";
        buf = buf.."<h1>Decentralized Task Assignment</h1>";
        buf = buf.."<h3>Agent</h3>";
        buf = buf.."<form role=\"form\">";
        buf = buf.."<div class=\"radio\">";
        buf = buf.."<label><input type=\"radio\" name=\"control\" value=\"off\" onclick=\"this.form.submit()\">off</input>";
        buf = buf.."<label><label><input type=\"radio\" name=\"control\" value=\"forward\" onclick=\"this.form.submit()\">forward</input>";
        buf = buf.."<label><label><input type=\"radio\" name=\"control\" value=\"backward\" onclick=\"this.form.submit()\">backward</input>";
        buf = buf.."<label><label><input type=\"radio\" name=\"control\" value=\"clockwise\" onclick=\"this.form.submit()\">clockwise</input>";
        buf = buf.."<label><label><input type=\"radio\" name=\"control\" value=\"counterclockwise\" onclick=\"this.form.submit()\">counterclockwise</input>";
        buf = buf.."</label></div>";
        buf = buf.."</form>";
        buf = buf.."</div>";
        buf = buf.."</html>"

if(_GET.control == "off")then
              off()
elseif(_GET.control == "forward")then
              forward()
elseif(_GET.control == "backward")then
              backward()          
elseif(_GET.control == "clockwise")then
              clockwise()
elseif(_GET.control == "counterclockwise")then
              counterclockwise()                                   
end

        client:send(buf)
        client:close()
        
collectgarbage()
end)
end)
