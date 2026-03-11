from flask import Flask, request, jsonify, render_template_string
from datetime import datetime
import pyttsx3
import pythoncom

app = Flask(__name__)

# ---------------- GLOBAL STATE ----------------

detected_text = ""
recent_sentences = []
current_language = "EN"

# ---------------- TTS FUNCTION ----------------

def speak_text(text, language):
    pythoncom.CoInitialize()
    engine = pyttsx3.init()

    if language == "ES":
        engine.setProperty(
            'voice',
            r'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\TTS_MS_ES-ES_HELENA_11.0'
        )
        engine.setProperty('rate', 165)  # Slightly slower for clarity
    else:
        engine.setProperty(
            'voice',
            r'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\TTS_MS_EN-US_DAVID_11.0'
        )
        engine.setProperty('rate', 175)

    engine.say(text)
    engine.runAndWait()
    engine.stop()

# ---------------- HTML PAGE ----------------

HTML_PAGE = """
<!DOCTYPE html>
<html>
<head>
<title>Braille Assistive System</title>
<style>
body { background:#111; color:white; font-family:Arial; text-align:center; }
.box { margin:20px auto; padding:20px; width:70%; background:#222; border-radius:10px; }
h1 { color:cyan; }
.live-text { font-size:28px; margin:15px; }
.recent { text-align:left; margin-top:20px; }
</style>
</head>
<body>

<h1>Braille Assistive System (English / Spanish)</h1>

<div class="box">
<h2>Live Detected Text</h2>
<div id="liveText" class="live-text"></div>
</div>

<div class="box recent">
<h3>Recent Sentences</h3>
<div id="recentList"></div>
</div>

<script>
function fetchData(){
fetch('/get_data')
.then(r=>r.json())
.then(data=>{
document.getElementById("liveText").innerText=data.live_text;

let html="";
data.recent.slice().reverse().forEach(item=>{
html+=`<p>[${item.time}] ${item.text}</p>`;
});
document.getElementById("recentList").innerHTML=html;
});
}
setInterval(fetchData,500);
fetchData();
</script>

</body>
</html>
"""

# ---------------- ROUTES ----------------

@app.route('/')
def index():
    return render_template_string(HTML_PAGE)

@app.route('/get_data')
def get_data():
    return jsonify({
        "live_text": detected_text,
        "recent": recent_sentences
    })

@app.route('/update')
def update_text():
    global detected_text, recent_sentences, current_language

    text = request.args.get('text', '')

    # ---- START ENGLISH ----
    if text == "START_EN":
        detected_text = ""
        current_language = "EN"
        return "Started English"

    # ---- START SPANISH ----
    elif text == "START_ES":
        detected_text = ""
        current_language = "ES"
        return "Started Spanish"

    # ---- GENERIC START (safety) ----
    elif text == "START":
        detected_text = ""
        return "Started"

    # ---- CLEAR ----
    elif text == "CLEAR":
        detected_text = ""
        return "Cleared"

    # ---- END (TRIGGER TTS) ----
    elif text == "END":
        if detected_text.strip() != "":
            speak_text(detected_text, current_language)

            timestamp = datetime.now().strftime("%H:%M:%S")
            recent_sentences.append({
                "time": timestamp,
                "text": detected_text
            })

        detected_text = ""
        return "TTS Triggered"

    # ---- NORMAL LETTER ----
    elif len(text) == 1 or text == " ":
        detected_text += text
        return "Letter Added"

    # ---- IGNORE UNKNOWN ----
    else:
        return "Ignored"

# ---------------- RUN ----------------

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
