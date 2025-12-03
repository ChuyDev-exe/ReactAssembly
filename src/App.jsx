import './App.css';
import { useState, useEffect, useRef } from 'react';

function App() {
  const [moduleReady, setModuleReady] = useState(false);
  const [error, setError] = useState(null);
  const canvasRef = useRef(null);

  useEffect(() => {
    let module = null;
    
    const loadWasm = () => {
      return new Promise((resolve, reject) => {
        // Set up the global Module object before loading the script
        window.Module = {
          canvas: canvasRef.current,
          onRuntimeInitialized: () => {
            setModuleReady(true);
            resolve(window.Module);
          },
          print: (text) => console.log('SDL Output:', text),
          printErr: (text) => console.error('SDL Error:', text),
          locateFile: (path, scriptDirectory) => {
            console.log('Locating file:', path, 'in:', scriptDirectory);
            if (path.endsWith('.wasm') || path.endsWith('.data')) {
              return '/wasm/' + path;
            }
            return scriptDirectory + path;
          },
          onAbort: (what) => {
            setError('WebAssembly module failed to load');
            reject(new Error('Module aborted: ' + what));
          }
        };

        // Load the script
        if (!document.getElementById('emscripten-wasm-script')) {
          const script = document.createElement('script');
          script.src = '/wasm/simple.js';
          script.async = true;
          script.id = 'emscripten-wasm-script';
          script.onload = () => {
            console.log('Script loaded successfully');
          };
          script.onerror = (err) => {
            console.error('Failed to load script:', err);
            setError('Failed to load WebAssembly script');
            reject(err);
          };
          document.body.appendChild(script);
        } else {
          // Script already loaded, check if module is ready
          if (window.Module && window.Module._main) {
            setModuleReady(true);
            resolve(window.Module);
          }
        }
      });
    };

    if (canvasRef.current && !moduleReady) {
      loadWasm().catch(err => {
        setError('Failed to initialize SDL WebAssembly module');
      });
    }

    return () => {
      // Cleanup if needed
      if (module && module._main) {
        // Any cleanup code
      }
    };
  }, [moduleReady]);

  return (
    <div className="w-full h-screen flex flex-col items-center justify-center bg-gray-900 text-white">
      <div className="relative">
        <canvas
          ref={canvasRef}
          id="canvas"
          width="300"
          height="300"
          style={{ 
            border: '2px solid #4a5568',
            borderRadius: '8px',
            boxShadow: '0 4px 15px rgba(0,0,0,0.3)'
          }}
        ></canvas>
        
        {!moduleReady && (
          <div className="absolute inset-0 flex items-center justify-center bg-black bg-opacity-70 rounded-lg">
            <div className="text-center">
              <div className="animate-spin rounded-full h-6 w-6 border-b-2 border-blue-400 mx-auto mb-2"></div>
              <p className="text-sm">Loading...</p>
            </div>
          </div>
        )}
      </div>

      {error ? (
        <div className="mt-4 p-3 bg-red-900 border border-red-700 rounded">
          <p className="text-red-300 text-sm">{error}</p>
        </div>
      ) : moduleReady ? (
        <div className="mt-4 text-center">

        </div>
      ) : (
        <p className="mt-4 text-gray-400 text-sm">Initializing SDL...</p>
      )}
    </div>
  );
}

export default App;
