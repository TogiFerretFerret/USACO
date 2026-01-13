browser.runtime.onMessage.addListener((message, sender, sendResponse) => {
    if (message.type === 'submit') {
        uploadData(message.name, message.url)
            .then(() => sendResponse({success: true}))
            .catch(err => {
                console.error(err);
                sendResponse({success: false, error: err.toString()});
            });
        return true; // Keep channel open for async response
    }
});

async function uploadData(name, url) {
    const response = await fetch("http://localhost:27182/submit", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            name: name,
            url: url
        })
    });

    if (!response.ok) {
        throw new Error("Server returned " + response.status);
    }
}
