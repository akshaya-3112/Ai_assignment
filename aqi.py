import csv

def calculate_aqi(pm25):
    if pm25 <= 50:
        return "Good"
    elif pm25 <= 100:
        return "Satisfactory"
    elif pm25 <= 200:
        return "Moderate"
    elif pm25 <= 300:
        return "Poor"
    elif pm25 <= 400:
        return "Very Poor"
    else:
        return "Severe"

def reflex_agent(file):
    with open(file) as f:
        reader = csv.DictReader(f)
        for row in reader:
            pm25 = int(row["PM2.5"])
            category = calculate_aqi(pm25)
            print("PM2.5:", pm25, " AQI:", category)

reflex_agent("aqi_data.csv")
