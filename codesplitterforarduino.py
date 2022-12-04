"""text = "5655188752B65F2B5E00FC3151503F940824CBF1FC5C8EA3F825856057"

totalbytes = len(text)//2

finalcode = ""

for i in range(0, totalbytes):
    finalcode = finalcode + "beacon_data[" + str(i) + "] = 0x" + text[2*i] + text[2*i+1] + ";" + "\r\n"

print(finalcode)"""


print("Bedroom")
text = "8456dc200388".upper()

totalbytes = len(text)//2

finalcode = ""

for i in range(0, totalbytes):
    finalcode = finalcode + "    beacon_data[" + str(i+21) + "] = 0x" + text[2*i] + text[2*i+1] + ";" + "\r\n"

print(finalcode)

print("Labroom")

text = "30e2e29458bc".upper()

totalbytes = len(text)//2

finalcode = ""

for i in range(0, totalbytes):
    finalcode = finalcode + "    beacon_data[" + str(i+21) + "] = 0x" + text[2*i] + text[2*i+1] + ";" + "\r\n"

print(finalcode)


print("Livroom")

text = "0fdd41abfab7".upper()

totalbytes = len(text)//2

finalcode = ""

for i in range(0, totalbytes):
    finalcode = finalcode + "    beacon_data[" + str(i+21) + "] = 0x" + text[2*i] + text[2*i+1] + ";" + "\r\n"

print(finalcode)