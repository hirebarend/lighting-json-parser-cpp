import fs from "node:fs/promises";

for (let i = 0; i < 10_000_000; i++) {
  await fs.appendFile(
    "data.jsonl",
    `${JSON.stringify({
      jsonrpc: "2.0",
      method: ["buy", "sell"][Math.random() >= 0.5 ? 0 : 1],
      params: ["BTC", (Math.random() * 1000).toFixed(2)],
      id: i,
    })}\n`
  );
}
