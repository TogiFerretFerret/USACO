browser.runtime.onMessage.addListener((message, sender, sendResponse) => {
    if (message.type === 'submit') {
        uploadData(message.name, message.zipBuffer)
            .then(() => sendResponse({success: true}))
            .catch(err => {
                console.error(err);
                sendResponse({success: false, error: err.toString()});
            });
        return true; // Keep channel open for async response
    }
});

async function uploadData(name, zipArray) {
    // zipArray is a standard array of bytes (from serialized Uint8Array)
    const uint8Array = new Uint8Array(zipArray);
    const blob = new Blob([uint8Array], {type: 'application/zip'});

    const response = await fetch("http://localhost:27182/submit", {
        method: "POST",
        headers: {
            "Content-Type": "application/zip",
            "X-Problem-Name": encodeURIComponent(name) // Encode to be safe in headers
        },
        body: blob
    });

    if (!response.ok) {
        throw new Error("Server returned " + response.status);
    }
}
