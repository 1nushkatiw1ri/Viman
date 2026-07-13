const { execFile } = require("child_process");
const path = require("path");

function executeAlgorithm(source, destination) {
    return new Promise((resolve, reject) => {

        const executable = path.join(
            __dirname,
            "../algorithms/graph.exe"
        );

        execFile(
            executable,
            [source, destination],
            {
                cwd: path.join(__dirname, "../algorithms")
            },
            (error, stdout, stderr) => {

                if (error) {
                    reject(stderr || error.message);
                    return;
                }

                try {
                    resolve(JSON.parse(stdout));
                } catch (e) {
                    reject(stdout);
                }

            }
        );

    });
}

module.exports = executeAlgorithm;