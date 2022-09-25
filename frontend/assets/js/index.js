const STADIUMS = new Map()
STADIUMS.set("Badminton court1", "5")
STADIUMS.set("Badminton court2", "6")
STADIUMS.set("Badminton court3", "7")
STADIUMS.set("Badminton court4", "8")
STADIUMS.set("Badminton court5", "13")
STADIUMS.set("Badminton court6", "14")

const TIMES = new Map()
TIMES.set("Badminton 15.5to16", "21")
TIMES.set("Badminton 15to16", "22")
TIMES.set("Badminton 16to17", "2")
TIMES.set("Badminton 17to18", "3")
TIMES.set("Badminton 18to19", "4")
TIMES.set("Badminton 19to20", "5")
TIMES.set("Badminton 20to21", "6")

function initOptions() {
  const stadiumOptions = this.document.querySelector(".stadium-options")
  const timeOptions = this.document.querySelector(".time-options")
  for (let i = 0, keys = STADIUMS.keys(); i < STADIUMS.size; i++) {
    let opt = document.createElement("option")
    opt.textContent = keys.next().value
    stadiumOptions.appendChild(opt)
  }
  for (let i = 0, keys = TIMES.keys(); i < TIMES.size; i++) {
    let opt = document.createElement("option")
    opt.textContent = keys.next().value
    timeOptions.appendChild(opt)
  }
}

function showRes(res) {
  const display = this.document.querySelector(".display")
  display.textContent = res
}

function sendBookRequest(data) {
  let request = new XMLHttpRequest()
  request.open("POST", '/book', true)
  request.send(JSON.stringify(data))
  request.onreadystatechange = function () {
    if (request.readyState == 4 && request.status == 200) {
      let res = request.responseText
      console.log(res)
    }
  }
}

function sendInfoRequest(data) {
  let request = new XMLHttpRequest()
  request.open("POST", '/info', true)
  request.send(JSON.stringify(data))
  request.onreadystatechange = function () {
    if (request.readyState == 4 && request.status == 200) {
      let res = request.responseText
      showRes(res)
    }
  }
}

window.addEventListener("load", function () {
  initOptions()
  this.document.querySelector(".ok-btn").addEventListener("mousedown", function() {
    let data = {
      "token" : document.querySelector(".token-ipt").value,
      "stadiumId": STADIUMS.get(document.querySelector(".stadium-select").value),
      "periodId": TIMES.get(document.querySelector(".time-select").value),
      "booktime": {
        "hour": document.querySelector(".hour-ipt").value,
        "min": document.querySelector(".min-ipt").value,
        "sec": document.querySelector(".sec-ipt").value,
        "millisec": document.querySelector(".millisec-ipt").value
      }
    }
    console.log(data)
    sendBookRequest(data)
    // setTimeout(() => {
    //   sendInfoRequest(data)
    // }, 1000)
  })
})