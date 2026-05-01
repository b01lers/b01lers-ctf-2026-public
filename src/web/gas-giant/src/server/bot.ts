import puppeteer from "puppeteer";

function sleep(m: number) {
  return new Promise((res) => setTimeout(res, m));
}

export async function visit(url: string) {
  const browser = await puppeteer.launch({
    args: ["--no-sandbox", "--disable-gpu", "--disable-setuid-sandbox"],
  });
  const page = await browser.newPage();

  // TODO: domain
  await browser.setCookie({
    name: "flag",
    value: "bctf{boys_go_to_jupiter_to_get_more_stupider}",
    domain: "localhost",
  });

  console.log("Visiting:", url);

  await page.goto(url);

  // Wait for the run button to load, then run the cell
  await page
    .locator("button.cursor-pointer.text-right.px-1")
    .setTimeout(120000)
    .click();
  await sleep(5000);

  console.log("Done visiting :D");
}
