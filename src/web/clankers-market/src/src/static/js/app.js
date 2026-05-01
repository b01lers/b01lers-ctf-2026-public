const victims = [
  "Aster Vex",
  "Bryn Cobalt",
  "Cipher Loom",
  "Delta Quill",
  "Echo Warden",
  "Nova Grain",
  "Piper Shade",
  "Rook Vent",
  "Silas Mint",
  "Tara Flux"
];

const providers = ["OpenAI", "Anthropic", "Google", "Mistral", "xAI", "Groq"];

const refreshSeconds = 20;
let secondsLeft = refreshSeconds;

function randomFrom(array) {
  return array[Math.floor(Math.random() * array.length)];
}

function randomString(length) {
  const alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  let out = "";
  for (let i = 0; i < length; i += 1) {
    out += alphabet[Math.floor(Math.random() * alphabet.length)];
  }
  return out;
}

function apiKeyFor(provider) {
  if (provider === "OpenAI") return `sk-openai-${randomString(26)}`;
  if (provider === "Anthropic") return `sk-ant-${randomString(24)}`;
  if (provider === "Google") return `AIza${randomString(35)}`;
  if (provider === "Mistral") return `mistral-${randomString(22)}`;
  if (provider === "xAI") return `xai-${randomString(20)}`;
  return `gsk_${randomString(28)}`;
}

function generateListings(count = 8) {
  const listings = [];
  for (let i = 0; i < count; i += 1) {
    const provider = randomFrom(providers);
    const price = (Math.random() * 2500 + 150).toFixed(2);
    listings.push({
      victim: randomFrom(victims),
      provider,
      apiKey: apiKeyFor(provider),
      price: `$${price}`
    });
  }
  return listings;
}

function renderListings() {
  const grid = document.getElementById("listing-grid");
  if (!grid) return;

  const listings = generateListings();
  grid.innerHTML = "";

  for (const listing of listings) {
    const card = document.createElement("article");
    card.className = "listing";
    card.innerHTML = `
      <h3>${listing.victim}</h3>
      <p>Provider: ${listing.provider}</p>
      <p>API Key: ${listing.apiKey}</p>
      <p class="price">Price: ${listing.price}</p>
    `;
    grid.appendChild(card);
  }
}

function updateTimer() {
  const timer = document.getElementById("refresh-timer");
  if (!timer) return;
  timer.textContent = String(secondsLeft);
}

function cycleListings() {
  renderListings();
  secondsLeft = refreshSeconds;
  updateTimer();
}

cycleListings();

setInterval(() => {
  secondsLeft -= 1;
  if (secondsLeft <= 0) {
    cycleListings();
    return;
  }
  updateTimer();
}, 1000);
