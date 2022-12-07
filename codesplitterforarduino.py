text = "0201021b0377f8b65f2b5e00fc315150b55a0824cb1dfc57f1f407174a8561"

totalbytes = len(text)//2

finalcode = ""

for i in range(0, totalbytes):
    finalcode = finalcode + "beacon_data[" + str(i) + "] = 0x" + text[2*i] + text[2*i+1] + ";" + "\r\n"

print(finalcode)

"""
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

print(finalcode)"""