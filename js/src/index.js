const os = require("node:os");
path = require("node:path");

const platform = os.platform();
const arch = os.arch();

const addonPath = path.resolve(__dirname, "../../addon", platform, arch, "node-watermark-loader.node");

console.info("Loading addon from", addonPath);

module.exports = require(addonPath);

