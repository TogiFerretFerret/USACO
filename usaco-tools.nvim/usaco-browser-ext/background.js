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

async function uploadData(name, zipBuffer) {
    // Convert ArrayBuffer back to Blob
    const blob = new Blob([zipBuffer], {type: 'application/zip'});
    
    const formData = new FormData();
    formData.append("name", name);
    formData.append("zip", blob, "testdata.zip");

    const response = await fetch("http://localhost:27182/submit", {
        method: "POST",
        body: formData
    });

    if (!response.ok) {
        throw new Error("Server returned " + response.status);
    }
}
