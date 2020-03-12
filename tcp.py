import webbrowser
root_url = "http://192.168.31.135ff"

def sendRequest(url):
    webbrowser.open(url, new=0)
    #end_headers()

while True:
    answer = input("To control type code:")
    print(answer)
    if (answer== "1"):
        sendRequest(root_url + "/3_MOTORS_ON")
        print("3 motors ON")

    elif (answer == "2"):
        sendRequest(root_url + "/5_MOTORS_ON")
        print ("5 motors ON")
        
    
    elif(answer == "OFF"):
        sendRequest(root_url + "/CLOSE_MOTORS")
        print ("MOTORS OFF")
