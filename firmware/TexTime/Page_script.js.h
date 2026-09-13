const char PAGE_microajax_js[] PROGMEM = R"=====(
class RequestQueue {
  constructor() {
    this.queue = [];
    this.processing = false;
    this.currentRequest = null;
  }

  add(url, callback, priority = 'normal') {
    return new Promise((resolve, reject) => {
      const request = {
        url,
        callback,
        priority,
        resolve,
        reject,
        timestamp: Date.now()
      };
      
      if (priority === 'high') {
        this.queue.unshift(request);
      } else {
        this.queue.push(request);
      }
      
      this.process();
    });
  }

  async process() {
    if (this.processing || this.queue.length === 0) return;
    
    this.processing = true;
    const request = this.queue.shift();
    this.currentRequest = request;

    try {
      const response = await this.fetch(request.url);
      this.parseResponse(response);
      if (request.callback) request.callback();
      request.resolve(response);
    } catch (error) {
      console.error('Request failed:', error);
      request.reject(error);
    } finally {
      this.currentRequest = null;
      this.processing = false;
      if (this.queue.length > 0) {
        setTimeout(() => this.process(), 50);
      }
    }
  }

  fetch(url) {
    return new Promise((resolve, reject) => {
      const xhr = new XMLHttpRequest();
      const fullUrl = url + (url.includes('?') ? '&' : '?') + 't=' + Date.now();
      
      xhr.onreadystatechange = function() {
        if (xhr.readyState === 4) {
          if (xhr.status === 200) {
            resolve(xhr.responseText);
          } else {
            reject(new Error(`HTTP ${xhr.status}: ${xhr.statusText}`));
          }
        }
      };
      
      xhr.timeout = 10000;
      xhr.ontimeout = () => reject(new Error('Request timeout'));
      xhr.onerror = () => reject(new Error('Network error'));
      
      xhr.open('GET', fullUrl, true);
      xhr.setRequestHeader('Cache-Control', 'no-cache');
      xhr.send();
    });
  }

  parseResponse(response) {
    if (!response) return;
    
    response.split('\n').forEach(entry => {
      const fields = entry.split('|');
      if (fields.length < 3) return;
      
      const [elementId, value, type] = fields;
      const element = document.getElementById(elementId);
      if (!element) return;

      try {
        switch(type) {
          case 'input':
            element.value = value;
            break;
          case 'div':
            element.innerHTML = value;
            break;
          case 'chk':
            element.checked = value === 'checked' || value === 'true';
            break;
          case 'jscolor':
            if (element.jscolor) {
              element.jscolor.fromString(value);
            }
            break;
          case 'select':
            // Check for duplicates before adding
            let optionExists = false;
            for (let i = 0; i < element.options.length; i++) {
              if (element.options[i].textContent === value) {
                optionExists = true;
                break;
              }
            }
            if (!optionExists && value.trim() !== '') {
              const option = document.createElement('option');
              option.value = element.options.length;
              option.textContent = value;
              element.appendChild(option);
            }
            break;
        }
      } catch (error) {
        console.error(`Failed to update element ${elementId}:`, error);
      }
    });
  }

  cancel() {
    this.queue = [];
    if (this.currentRequest && this.currentRequest.xhr) {
      this.currentRequest.xhr.abort();
    }
  }
}

const requestQueue = new RequestQueue();

function setValues(url, callback) {
  return requestQueue.add(url, callback);
}

function setValuesHighPriority(url, callback) {
  return requestQueue.add(url, callback, 'high');
}

function load(src, type, callback) {
  if (type === 'js') {
    const script = document.createElement('script');
    script.src = src;
    script.type = 'text/javascript';
    script.async = false;
    script.onload = () => callback && callback();
    script.onerror = () => console.error(`Failed to load script: ${src}`);
    document.head.appendChild(script);
  } else if (type === 'css') {
    const link = document.createElement('link');
    link.href = src;
    link.rel = 'stylesheet';
    link.type = 'text/css';
    link.onload = () => callback && callback();
    link.onerror = () => console.error(`Failed to load stylesheet: ${src}`);
    document.head.appendChild(link);
  }
}

function showLoading(elementId) {
  const element = document.getElementById(elementId);
  if (element) {
    element.innerHTML = '<div class="loading">Loading...</div>';
  }
}

function hideLoading(elementId) {
  const element = document.getElementById(elementId);
  if (element && element.querySelector('.loading')) {
    element.innerHTML = '';
  }
}

function debounce(func, wait) {
  let timeout;
  return function executedFunction(...args) {
    const later = () => {
      clearTimeout(timeout);
      func(...args);
    };
    clearTimeout(timeout);
    timeout = setTimeout(later, wait);
  };
}

// jscolor library removed - using native HTML5 color picker instead

)=====";
