const {spawnSync} = require("node:child_process");
const os = require("node:os");
const path = require("node:path");

const args = ["rebuild", "-T", "install", ...process.argv.slice(2)]


const cmakeDefineProperties = {
    NODE_ADDON_INSTALL_PREFIX: `"${path.resolve(__dirname, "../addon")}"`,
    NODE_ADDON_PLATFORM: os.platform(),
    NODE_ADDON_ARCH: os.arch(),
}

args.push(...
    Object.entries(cmakeDefineProperties)
        .map(([key, value]) => `--CD${key}=${value}`)
)

spawnSync("cmake-js", args, {
    stdio: "inherit",
    shell: true,
    windowsVerbatimArguments: false
})