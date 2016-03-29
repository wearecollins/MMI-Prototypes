/* globals Loader, mat4 */

/**
 * handles incoming image stream and displays it in background
 * @constructor
 */
function StreamHandler(){

  var domElem;
  var prevURL;
  var active = false;
  var ready = false;
  //FROM NEHE
  var gl;
  var neheTexture;

  /**
   * initializes the StreamHandler
   * @param {WebsocketHandler} streamSource 
   *  StreamHandler will register itself with this source
   * @param {Element} domDestination
   *  DOM element to use for displaying the image stream
   * @returns {Promise<StreamHandler>} resolves to this instance
   */
  this.init = function init(streamSource, domDestination){
    var fragPromise = Loader.loadType('js/shaders/basic.frag', 
                                      Loader.MIME.frag);
    var vertPromise = Loader.loadType('js/shaders/basic.vert', 
                                      Loader.MIME.vert);
    var matPromise = Loader.loadJS('js/vendor/glMatrix-0.9.5.min.js').
      then( function resolve(){
        mvMatrix = mat4.create();
        pMatrix = mat4.create();
      });
    return Promise.all([fragPromise, vertPromise, matPromise]).
      then( function resolve(results){
        streamSource.addBinaryHandler(handleImage);

        // for img-based
        //domElem = document.createElement('img');
        
        // for WebGL-based
        domElem = document.createElement('canvas');

        // common
        domDestination.appendChild(domElem);

        // for WebGL-based
        //setupWebGL(domElem);
        initGL(domElem);
        initShaders(...results);
        initBuffers();
        initTexture();

        gl.clearColor(1.0, 0.0, 1.0, 1.0);

        tick();
        ready = true;
      }).
      then( ( () => this ).bind(this) );
  };

  this.showStream = function showStream(){
    active = true;
    log.warn('TODO: show stream!');
  };

  this.hideStream = function hideStream(){
    active = false;
    log.warn('TODO: hide stream!');
  };

  //images come as binary from the server
  // lets just dump them into an <img>
  function handleImage(buf){
    if (!(active && ready)){
      return;
    }
    var view = new Uint8Array(buf);
    var blob = new Blob([view], {type: 'image/jpeg'});
    var url = window.URL.createObjectURL(blob);

    // for img-based
    //domElem.src = url;
    
    // for WebGL-based
    var image = new Image();
    image.onload = function(){
      image.onload = undefined;
      image.onerror = undefined;
      updateLoadedTexture(neheTexture, image);
    };
    image.onerror = function(e){
      log.error('[StreamHandler::handleImage] problem loading image', e);
      //document.location.reload(true);
    };
    image.src = window.webkitURL.createObjectURL(blob);

    // common
    if (prevURL !== undefined){
      window.URL.revokeObjectURL(prevURL);
    }
    prevURL = url;
  }

  var cubeVertexPositionBuffer;
  var cubeVertexTextureCoordBuffer;
  var cubeVertexIndexBuffer;

  function initBuffers() {
    cubeVertexPositionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexPositionBuffer);
    /*eslint-disable indent*/
    var vertices = [
      // Front face
      -1.0, -1.0,  0.0,
       1.0, -1.0,  0.0,
       1.0,  1.0,  0.0,
      -1.0,  1.0,  0.0
    ];
    /*eslint-enable*/
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
    cubeVertexPositionBuffer.itemSize = 3;
    cubeVertexPositionBuffer.numItems = 12;

    cubeVertexTextureCoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexTextureCoordBuffer);
    var textureCoords = [
      // Front face
      0.0, 0.0,
      1.0, 0.0,
      1.0, 1.0,
      0.0, 1.0
    ];
    gl.bufferData(gl.ARRAY_BUFFER, 
                  new Float32Array(textureCoords), 
                  gl.STATIC_DRAW);
    cubeVertexTextureCoordBuffer.itemSize = 2;
    cubeVertexTextureCoordBuffer.numItems = 8;

    cubeVertexIndexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
    var cubeVertexIndices = [
      0, 1, 2, 0, 2, 3    // Front face
    ];
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, 
                  new Uint16Array(cubeVertexIndices), 
                  gl.STATIC_DRAW);
    cubeVertexIndexBuffer.itemSize = 1;
    cubeVertexIndexBuffer.numItems = 6;
  }

  function tick() {
    //window.webkitRequestAnimationFrame(tick);
    console.time('render');
    drawScene();
    console.timeEnd('render');
  }
  function degToRad(degrees) {
    return degrees * Math.PI / 180;
  }
  var xRot = 0;
  var yRot = 0;
  var zRot = 0;
  var mvMatrix;
  var mvMatrixStack = [];
  var pMatrix;
  var shaderProgram;


  function webGLStart() {
    var canvas = document.getElementById('canvas');
    initGL(canvas);
    initShaders();
    initBuffers();
    initTexture();

    gl.clearColor(1.0, 0.0, 1.0, 1.0);

    tick();
  }

  function drawScene() {
    gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    //mat4.ortho(-2,2,-2,2,-2,2,pMatrix);
    mat4.perspective(22.6/*45*/, 
                     1/*gl.viewportWidth / gl.viewportHeight*/, 
                     0.1, 
                     100.0, 
                     pMatrix);

    mat4.identity(mvMatrix);

    mat4.translate(mvMatrix, [0.0, 0.0, -5.0]);

    mat4.rotate(mvMatrix, degToRad(xRot), [1, 0, 0]);
    mat4.rotate(mvMatrix, degToRad(yRot), [0, 1, 0]);
    mat4.rotate(mvMatrix, degToRad(zRot), [0, 0, 1]);

    gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexPositionBuffer);
    gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, 
                           cubeVertexPositionBuffer.itemSize, 
                           gl.FLOAT, 
                           false, 
                           0, 
                           0);

    gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexTextureCoordBuffer);
    gl.vertexAttribPointer(shaderProgram.textureCoordAttribute, 
                           cubeVertexTextureCoordBuffer.itemSize, 
                           gl.FLOAT, 
                           false, 
                           0, 
                           0);

    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, neheTexture);
    gl.uniform1i(shaderProgram.samplerUniform, 0);

    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
    setMatrixUniforms();
    gl.drawElements(gl.TRIANGLES, 
                    cubeVertexIndexBuffer.numItems, 
                    gl.UNSIGNED_SHORT, 
                    0);
  }

  function setMatrixUniforms() {
    gl.uniformMatrix4fv(shaderProgram.pMatrixUniform, false, pMatrix);
    gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);
  }

  function mvPushMatrix() {
    var copy = mat4.create();
    mat4.set(mvMatrix, copy);
    mvMatrixStack.push(copy);
  }

//FROM NEHE

  function initGL(canvas) {
    try {
      gl = canvas.getContext('webgl') || 
           canvas.getContext('experimental-webgl') ||
           canvas.getContext('moz-webgl');
      gl.viewportWidth = canvas.width;
      gl.viewportHeight = canvas.height;
    } catch (e) {
      log.error(e);
    }
    if (!gl) {
      alert('Could not initialise WebGL, sorry :-(');
    }
  }

  function compileVert(gl, vertText){
    var shader = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(shader, vertText);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
      alert(gl.getShaderInfoLog(shader));
      return null;
    }

    return shader;
  }

  function compileFrag(gl, fragText){
    var shader = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(shader, fragText);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
      alert(gl.getShaderInfoLog(shader));
      return null;
    }

    return shader;
  }

  function getShader(gl, id) {
    var shaderScript = document.getElementById(id);
    if (!shaderScript) {
      return null;
    }

    var str = '';
    var k = shaderScript.firstChild;
    while (k) {
      if (k.nodeType == 3) {
        str += k.textContent;
      }
      k = k.nextSibling;
    }

    var shader;
    if (shaderScript.type == 'x-shader/x-fragment') {
      shader = gl.createShader(gl.FRAGMENT_SHADER);
    } else if (shaderScript.type == 'x-shader/x-vertex') {
      shader = gl.createShader(gl.VERTEX_SHADER);
    } else {
      return null;
    }

    gl.shaderSource(shader, str);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
      alert(gl.getShaderInfoLog(shader));
      return null;
    }

    return shader;
  }



  /**
   * initialized the WebGL shaders
   * @param {string} [fragmentShader] text for the fragment shader.
   *  otherwise it is loaded from a script tag named "shader-fs".
   * @param {string} [vertexShader] text for the vertex shader.
   *  otherwise it is loaded from a script tag named "shader-vs".
   */
  function initShaders(fragmentShader, vertexShader) {
    fragmentShader = fragmentShader ?
                     compileFrag(gl, fragmentShader) :
                     getShader(gl, 'shader-fs');
    vertexShader = vertexShader ?
                   compileVert(gl, vertexShader) :
                   getShader(gl, 'shader-vs');

    shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
      alert('Could not initialise shaders');
    }

    gl.useProgram(shaderProgram);

    shaderProgram.vertexPositionAttribute = 
      gl.getAttribLocation(shaderProgram, 'aVertexPosition');
    gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);

    shaderProgram.textureCoordAttribute = 
      gl.getAttribLocation(shaderProgram, 'aTextureCoord');
    gl.enableVertexAttribArray(shaderProgram.textureCoordAttribute);

    shaderProgram.pMatrixUniform = 
      gl.getUniformLocation(shaderProgram, 'uPMatrix');
    shaderProgram.mvMatrixUniform = 
      gl.getUniformLocation(shaderProgram, 'uMVMatrix');
    shaderProgram.samplerUniform = 
      gl.getUniformLocation(shaderProgram, 'uSampler');
  }

  function initTexture() {
    neheTexture = gl.createTexture();
    neheTexture.image = new Image();
    neheTexture.image.onload = function () {
      handleLoadedTexture(neheTexture);
    };

    neheTexture.image.src = 
      'hall_and_oates_02.jpeg';
      //"nehe.gif";
      //"6020531481_e413735ea8_b.jpeg";
  }

  function handleLoadedTexture(texture) {
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
    gl.texImage2D(gl.TEXTURE_2D, 
                  0, 
                  gl.RGB, 
                  gl.RGB, 
                  gl.UNSIGNED_BYTE, 
                  texture.image);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.bindTexture(gl.TEXTURE_2D, null);
  }
  
  function updateLoadedTexture(texture, image){
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image);
    texture.image = image;
    gl.bindTexture(gl.TEXTURE_2D, null);
    tick();
  }

  /**
   * Creates a webgl context. If creation fails it will
   * change the contents of the container of the <canvas>
   * tag to an error message with the correct links for WebGL.
   * @param {Element} canvas. The canvas element to create a
   *     context from.
   * @param {WebGLContextCreationAttirbutes} opt_attribs Any
   *     creation attributes you want to pass in.
   * @param {function:(msg)} opt_onError An function to call
   *     if there is an error during creation.
   * @return {WebGLRenderingContext} The created context.
   */
  function setupWebGL(canvas, opt_attribs, opt_onError) {
    function handleCreationError(msg) {
      var container = canvas.parentNode;
      if (container) {
        var str = window.WebGLRenderingContext ?
             'there was some problem creating a WebGL context' :
             'this browser does not support WebGL';
        if (msg) {
          str += '<br/><br/>Status: ' + msg;
        }
        container.innerHTML = makeFailHTML(str);
      }
    }
  
    opt_onError = opt_onError || handleCreationError;
  
    if (canvas.addEventListener) {
      canvas.addEventListener('webglcontextcreationerror', 
                              function(event) {
                                opt_onError(event.statusMessage);
                              },
                              false);
    }
    var context = create3DContext(canvas, opt_attribs);
    if (!context) {
      if (!window.WebGLRenderingContext) {
        opt_onError('');
      }
    }
    return context;
  }
  
  /**
   * Creates a webgl context.
   * @param {!Canvas} canvas The canvas tag to get context
   *     from. If one is not passed in one will be created.
   * @return {!WebGLContext} The created context.
   */
  function create3DContext(canvas, opt_attribs) {
    var names = ['webgl', 'experimental-webgl', 'webkit-3d', 'moz-webgl'];
    var context = null;
    for (var ii = 0; ii < names.length; ++ii) {
      try {
        context = canvas.getContext(names[ii], opt_attribs);
      } catch(e) {}
      if (context) {
        break;
      }
    }
    return context;
  }
}
