(function(ext) {
    var device = null;
    var _rxBuf = [];
	var pin = {"D2":2,"D3":3,"D4":4,"D5":5,"D6":6,"D7":7,"D8":8,"D9":9,"D10":10,"D11":11,"D12":12,"A0":14,"A1":15,"A2":16,"A3":17};
    var axis = {"X":0,"Y":1,"Z":2};
    var motor = {"M1":1,"M2":0};
    var status = {"ON":1,"OFF":0};
    var row = {"LINE1":0,"LINE2":1};
    var servo = {"D2":2,"D3":3,"D4":4,"D5":5,"D6":6,"D7":7,"D8":8,"D9":9,"D10":10,"D11":11,"D12":12};
    var pinA = {"A0":14,"A1":15,"A2":16,"A3":17};
    ext.resetAll = function(){};
  
    ext.runArduino = function(){
        responseValue();
    };
    //////////////////////////////////////////////
    ext.setMotor = function(M,speed){
        runPackage(60,short2array(typeof M=="number"?M:motor[M]),short2array(speed));
    };
    ext.stopMotor = function(M){
        runPackage(61,short2array(typeof M=="number"?M:motor[M]));
    };
    ext.setRelay = function(p,stt){
        runPackage(62,short2array(typeof p=="number"?p:pin[p]),short2array(typeof stt=="number"?stt:status[stt]));
    };
    ext.setLED = function(p,stt){
        runPackage(63,short2array(typeof p=="number"?p:pin[p]),short2array(typeof stt=="number"?stt:status[stt]));
    };
    ext.setServo = function(sv,angle){
        runPackage(64,short2array(typeof sv=="number"?sv:servo[sv]),short2array(angle));
    };
    ext.disableServo = function(p){
        runPackage(65,short2array(typeof p=="number"?p:pin[p]));
    };
    //ext.LCDprint = function(rows,pos,strings){
    //    runPackage(66,short2array(typeof rows=="number"?rows:row[rows]),short2array(pos),)
    //};
    //ext.LCDclear = function(){
    //    runPackage(67);
    //};
    //////////////////////////////////////////////
    ext.buttonPressed = function(nextID,p){
        var deviceId = 70;
        getPackage(nextID,deviceId,typeof p=="number"?p:pin[p]);
    };
    ext.readMicroswitch = function(nextID,p){
        var deviceId = 71;
        getPackage(nextID,deviceId,typeof p=="number"?p:pin[p]);
    };
    ext.readTouch = function(nextID,p){
        var deviceId = 72;
        getPackage(nextID,deviceId,typeof p=="number"?p:pin[p]);
    };
    ext.readVibration = function(nextID,p){
        var deviceId = 73;
        getPackage(nextID,deviceId,typeof p=="number"?p:pin[p]);
    };
    ext.getGasSensor = function(nextID,p){
        var deviceId = 74;
        getPackage(nextID,deviceId,typeof p=="number"?p:pinA[p]);
    };
    ext.getSound = function(nextID,p){
        var deviceId = 75;
        getPackage(nextID,deviceId,typeof p=="number"?p:pinA[p]);
    };
    ext.getLight = function(nextID,p){
        var deviceId = 76;
        getPackage(nextID,deviceId,typeof p=="number"?p:pinA[p]);
    };
    ext.getSoilMoisture = function(nextID,p){
        var deviceId = 77;
        getPackage(nextID,deviceId,typeof p=="number"?p:pinA[p]);
    };
    ext.getPotentiometer = function(nextID,p){
        var deviceId = 78;
        getPackage(nextID,deviceId,typeof p=="number"?p:pinA[p]);
    };
    ext.getAcceleromenterValue = function(nextID,axiss){
        var deviceId = 79;
        getPackage(nextID,deviceId,typeof axiss=="number"?axiss:axis[axiss]);
    };
    ext.getTemperature = function(nextID,p){
        var deviceId = 80;
        getPackage(nextID,deviceId,typeof p=="number"?p:pin[p]);
    };
    ext.getHumidity = function(nextID,p){
        var deviceId = 81;
        getPackage(nextID,deviceId,typeof p=="number"?p:pin[p]);
    };
    //////////////////////////////////////////////
    function sendPackage(argList, type){
        var bytes = [0xff, 0x55, 0, 0, type];
        for(var i=0;i<argList.length;++i){
            var val = argList[i];
            if(val.constructor == "[class Array]"){
            bytes = bytes.concat(val);
            }
            else{
                bytes.push(val);
            }
        }
        bytes[2] = bytes.length - 3;
        device.send(bytes);
    }
      
    function runPackage(){
        sendPackage(arguments, 2);
    }
    function getPackage(){
        var nextID = arguments[0];
        Array.prototype.shift.call(arguments);
        sendPackage(arguments, 1);
    }
    
    var inputArray = [];
    var _isParseStart = false;
    var _isParseStartIndex = 0;

    //////////////////////////////////////////////
    function processData(bytes) {
        var len = bytes.length;
        if(_rxBuf.length>30){
            _rxBuf = [];
        }
        for(var index=0;index<bytes.length;index++){
            var c = bytes[index];
            _rxBuf.push(c);
            if(_rxBuf.length>=2){
                if(_rxBuf[_rxBuf.length-1]==0x55 && _rxBuf[_rxBuf.length-2]==0xff){
                    _isParseStart = true;
                    _isParseStartIndex = _rxBuf.length-2;
                }
                if(_rxBuf[_rxBuf.length-1]==0xa && _rxBuf[_rxBuf.length-2]==0xd&&_isParseStart){
                    _isParseStart = false;
          
                    var position = _isParseStartIndex+2;
                    var extId = _rxBuf[position];
                    position++;
                    var type = _rxBuf[position];
                    position++;
                    //1 byte 2 float 3 short 4 len+string 5 double
                    var value;
                    switch(type){
                        case 1:{
                            value = _rxBuf[position];
                            position++;
                        }
                        break;
                        case 2:{
                            value = readFloat(_rxBuf,position);
                            position+=4;
                            if(value<-255||value>1023){
                                value = 0;
                            }
                        }
                        break;
                        case 3:{
                            value = readInt(_rxBuf,position,2);
                            position+=2;
                        }
                        break;
                        case 4:{
                            var l = _rxBuf[position];
                            position++;
                            value = readString(_rxBuf,position,l);
                        }
                        break;
                        case 5:{
                            value = readDouble(_rxBuf,position);
                            position+=4;
                        }
                        break;
                        case 6:
                            value = readInt(_rxBuf,position,4);
                            position+=4;
                        break;
                    }
                    if(type<=6){
                        responseValue(extId,value);
                    }
                    else{
                        responseValue();
                    }
                    _rxBuf = [];
                }
            } 
        }
    }
    function readFloat(arr,position){
        var f= [arr[position],arr[position+1],arr[position+2],arr[position+3]];
        return parseFloat(f);
    }
    function readInt(arr,position,count){
        var result = 0;
        for(var i=0; i<count; ++i){
            result |= arr[position+i] << (i << 3);
        }
        return result;
    }
    function readDouble(arr,position){
        return readFloat(arr,position);
    }
    function readString(arr,position,len){
        var value = "";
        for(var ii=0;ii<len;ii++){
            value += String.fromCharCode(_rxBuf[ii+position]);
        }
        return value;
    }
    function appendBuffer( buffer1, buffer2 ) {
        return buffer1.concat( buffer2 );
    }

    // Extension API interactions
    var potentialDevices = [];
    ext._deviceConnected = function(dev) {
        potentialDevices.push(dev);

        if (!device) {
            tryNextDevice();
        }
    }

    function tryNextDevice() {
        // If potentialDevices is empty, device will be undefined.
        // That will get us back here next time a device is connected.
        device = potentialDevices.shift();
        if (device) {
            device.open({ stopBits: 0, bitRate: 115200, ctsFlowControl: 0 }, deviceOpened);
        }
    }

    function deviceOpened(dev) {
        if (!dev) {
            // Opening the port failed.
            tryNextDevice();
            return;
        }
        device.set_receive_handler('MakerKit',function(data) {
            processData(data);
        });
    };

    ext._deviceRemoved = function(dev) {
        if(device != dev) return;
        device = null;
    };

    ext._shutdown = function() {
        if(device) device.close();
        device = null;
    };

    ext._getStatus = function() {
        if(!device) return {status: 1, msg: 'NEGENDO disconnected'};
        return {status: 2, msg: 'NEGENDO connected'};
    }

    var descriptor = {};
	ScratchExtensions.register('MakerKit', descriptor, ext, {type: 'serial'});
})({});