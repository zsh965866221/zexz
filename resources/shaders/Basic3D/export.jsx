
basic_3d = function (aeProps, layerVar) {
  this.effectID = 33;
  this.aeProps = aeProps;
  this.wasLast = 0;
  this.passNum = 1;
  this.vshCode = 0;
  this.fshCode = 0;
};

basic_3d.prototype = {
  constructor: basic_3d,

  makeUniform: function (passIndex) {
    var ufm = new Array();
    ufm.push('uSwivel');
    ufm.push('uTilt');
    ufm.push('uDistance');
    ufm.push('uSpecular');
    return ufm;
  },

  markVarying: function (passIndex, layerVar) {
    var swivelProp = this.aeProps.property('Swivel');
    var tiltProp = this.aeProps.property('Tilt');
    var distanceProp = this.aeProps.property('Distance to Image');
    var specularProp = this.aeProps.property('Specular Highlight');

    var u0 = false;
    if ('undefined' !== swivelProp && null != swivelProp) {
      u0 = swivelProp.isTimeVarying;
    }
    var u1 = false;
    if ('undefined' !== tiltProp && null != tiltProp) {
      u1 = tiltProp.isTimeVarying;
    }
    var u2 = false;
    if ('undefined' !== distanceProp && null != distanceProp) {
      u2 = distanceProp.isTimeVarying;
    }
    var u3 = false;
    if ('undefined' !== specularProp && null != specularProp) {
      u3 = specularProp.isTimeVarying;
    }

    var tv = [u0, u1, u2, u3];
    return tv;
  },

  vshPassCode: function (passIndex) {
    return this.vshCode;
  },

  fshPassCode: function (passIndex) {
    return this.fshCode;
  },

  uniformData: function (atTime, layerVar, passIndex) {
    var swivelProp = this.aeProps.property('Swivel');
    var tiltProp = this.aeProps.property('Tilt');
    var distanceProp = this.aeProps.property('Distance to Image');
    var specularProp = this.aeProps.property('Specular Highlight');

    if ('undefined' === typeof(swivelProp) || 
        'undefined' === typeof(tiltProp) ||
        'undefined' === typeof(distanceProp) ||
        'undefined' === typeof(specularProp)) {
      return;
    }

    var swivel = swivelProp.valueAtTime(atTime, false);
    var tilt = tiltProp.valueAtTime(atTime, false);
    var distance = distanceProp.valueAtTime(atTime, false);
    var specular = specularProp.valueAtTime(atTime, false);

    var valueArray = new Array(4);
    valueArray[0] = swivel;
    valueArray[1] = tilt;
    valueArray[2] = distance;
    valueArray[3] = specular;

    return valueArray;
  }
};


var compVar = app.project.activeItem;
var layerVar = compVar.layer(1);    
var effects = layerVar.Effects;
var b3dProps = effects.property("Basic 3D");
var shader = new basic_3d(b3dProps, layerVar);
var ud = shader.uniformData(5000, layerVar, 0);
console("");