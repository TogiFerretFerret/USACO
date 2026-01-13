(function() {
    if (window.hasRun) {
        return;
    }
    window.hasRun = true;

    function createButton() {
        const testDataLinks = document.querySelectorAll('a[href$=".zip"]');
        
        testDataLinks.forEach(link => {
            if (link.textContent.toLowerCase().includes("test data")) {
                const button = document.createElement("button");
                button.textContent = "Send to Neovim";
                button.style.marginLeft = "10px";
                button.style.padding = "2px 8px";
                button.style.cursor = "pointer";
                button.style.backgroundColor = "#28a745";
                button.style.color = "white";
                button.style.border = "none";
                button.style.borderRadius = "4px";
                button.style.fontSize = "12px";

                button.onclick = (e) => {
                    e.preventDefault();
                    sendToNvim(link, button);
                };

                link.parentNode.insertBefore(button, link.nextSibling);
            }
        });
    }

    async function sendToNvim(link, button) {
        const originalText = button.textContent;
        button.textContent = "Sending...";
        button.disabled = true;

        try {
            // 1. Get Problem Name
            let container = link.closest('.panel');
            let problemName = "Unknown";
            
            if (container && container.classList.contains('historypanel')) {
                const bTag = container.querySelector('b');
                if (bTag) problemName = bTag.textContent.trim();
            } else {
                 const h2 = document.querySelector('div.panel h2');
                 if (h2) problemName = h2.textContent.trim();
            }

            // 2. Send URL to Background Script
            const res = await browser.runtime.sendMessage({
                type: 'submit',
                name: problemName,
                url: link.href
            });

            if (res && res.success) {
                button.textContent = "Sent!";
                setTimeout(() => {
                    button.textContent = originalText;
                    button.disabled = false;
                }, 2000);
            } else {
                throw new Error(res ? res.error : "Unknown error");
            }

        } catch (err) {
            console.error(err);
            button.textContent = "Error";
            button.style.backgroundColor = "#dc3545";
            setTimeout(() => {
                button.textContent = originalText;
                button.style.backgroundColor = "#28a745";
                button.disabled = false;
            }, 3000);
        }
    }

    createButton();

})();
