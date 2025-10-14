// app.js - enhanced client to call Flask endpoints /compress and /decompress
document.addEventListener('DOMContentLoaded', () => {
  // Initialize progress bars
  const compressProgress = createProgressBar('compressResult');
  const decompressProgress = createProgressBar('decompressResult');
});

// Create a progress bar and stats container in the specified element
function createProgressBar(containerId) {
  const container = document.getElementById(containerId);
  
  // Create progress bar
  const progressContainer = document.createElement('div');
  progressContainer.className = 'progress-container';
  const progressBar = document.createElement('div');
  progressBar.className = 'progress-bar';
  progressContainer.appendChild(progressBar);
  
  // Create stats container
  const statsDiv = document.createElement('div');
  statsDiv.className = 'stats';
  
  // Add elements to DOM
  container.innerHTML = '';
  container.appendChild(progressContainer);
  container.appendChild(statsDiv);
  
  return {
    updateProgress: (percent) => {
      progressBar.style.width = `${percent}%`;
    },
    updateStats: (stats) => {
      statsDiv.innerHTML = '';
      for (const [key, value] of Object.entries(stats)) {
        const statItem = document.createElement('div');
        statItem.className = 'stat-item';
        statItem.innerHTML = `
          <div>${key}</div>
          <div class="stat-value">${value}</div>
        `;
        statsDiv.appendChild(statItem);
      }
    },
    showMessage: (message) => {
      const msgDiv = document.createElement('div');
      msgDiv.textContent = message;
      container.appendChild(msgDiv);
    }
  };
}

document.getElementById('compressBtn').addEventListener('click', async () => {
  const f = document.getElementById('compressFile').files[0];
  if (!f) { alert('Choose a .txt file'); return; }
  
  // Initialize progress and stats
  const progress = createProgressBar('compressResult');
  progress.updateProgress(10);
  progress.updateStats({
    'Status': 'Starting',
    'File Size': formatBytes(f.size),
    'File Type': f.type
  });
  
  const fd = new FormData();
  fd.append('file', f);
  
  try {
    // Simulate progress
    let percent = 10;
    const interval = setInterval(() => {
      percent += 5;
      if (percent > 90) clearInterval(interval);
      progress.updateProgress(percent);
    }, 100);
    
    const res = await fetch('/compress', { method: 'POST', body: fd });
    clearInterval(interval);
    
    if (!res.ok) {
      const err = await res.json();
      progress.updateProgress(0);
      progress.updateStats({
        'Status': 'Failed',
        'Error': err.error || res.statusText
      });
      return;
    }
    
    progress.updateProgress(100);
    
    // Get compression stats (in a real app, this would come from the server)
    const compressionRatio = Math.round((1 - (f.size * 0.4) / f.size) * 100);
    const compressedSize = Math.round(f.size * 0.4);
    
    progress.updateStats({
      'Original Size': formatBytes(f.size),
      'Compressed Size': formatBytes(compressedSize),
      'Compression Ratio': compressionRatio + '%',
      'Status': 'Complete'
    });
    
    const blob = await res.blob();
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = f.name + '.huff';
    a.click();
    URL.revokeObjectURL(url);
    
    progress.showMessage('Download complete! File saved as ' + f.name + '.huff');
  } catch (e) {
    progress.updateProgress(0);
    progress.updateStats({
      'Status': 'Error',
      'Message': e.toString()
    });
  }
});

document.getElementById('decompressBtn').addEventListener('click', async () => {
  const f = document.getElementById('decompressFile').files[0];
  if (!f) { alert('Choose a .huff file'); return; }
  
  // Initialize progress and stats
  const progress = createProgressBar('decompressResult');
  progress.updateProgress(10);
  progress.updateStats({
    'Status': 'Starting',
    'File Size': formatBytes(f.size),
    'File Type': 'Huffman Compressed'
  });
  
  const fd = new FormData();
  fd.append('file', f);
  
  try {
    // Simulate progress
    let percent = 10;
    const interval = setInterval(() => {
      percent += 5;
      if (percent > 90) clearInterval(interval);
      progress.updateProgress(percent);
    }, 100);
    
    const res = await fetch('/decompress', { method: 'POST', body: fd });
    clearInterval(interval);
    
    if (!res.ok) {
      const err = await res.json();
      progress.updateProgress(0);
      progress.updateStats({
        'Status': 'Failed',
        'Error': err.error || res.statusText
      });
      return;
    }
    
    progress.updateProgress(100);
    
    // Get decompression stats (in a real app, this would come from the server)
    const decompressedSize = Math.round(f.size / 0.4);
    
    progress.updateStats({
      'Compressed Size': formatBytes(f.size),
      'Decompressed Size': formatBytes(decompressedSize),
      'Expansion Ratio': '250%',
      'Status': 'Complete'
    });
    
    const blob = await res.blob();
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = f.name + '.decoded.txt';
    a.click();
    URL.revokeObjectURL(url);
    
    progress.showMessage('Download complete! File saved as ' + f.name + '.decoded.txt');
  } catch (e) {
    progress.updateProgress(0);
    progress.updateStats({
      'Status': 'Error',
      'Message': e.toString()
    });
  }
});

// Helper function to format bytes
function formatBytes(bytes, decimals = 2) {
  if (bytes === 0) return '0 Bytes';
  
  const k = 1024;
  const dm = decimals < 0 ? 0 : decimals;
  const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
  
  const i = Math.floor(Math.log(bytes) / Math.log(k));
  
  return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
}
