on({id: "mqtt.0.stereoanlage.data.0", change: "ne"}, function (obj) {
    var value = obj.state.val;
    value = Math.round(value/1024*100);
    setState( "mqtt.0.pi_keller.funktionen.brightness",value);
});
on({id: "mqtt.0.stereoanlage.data.1", change: "ne"}, function (obj) {
    var value = obj.state.val;
    var rgb = getRGBVals();
    value = Math.round(value/1024*255);
    var out = value+","+rgb[1]+","+rgb[2];
    setState( "mqtt.0.pi_keller.funktionen.rgb",out);
});
on({id: "mqtt.0.stereoanlage.data.2", change: "ne"}, function (obj) {
    var value = obj.state.val;
    var rgb = getRGBVals();
    value = Math.round(value/1024*255);
    var out = rgb[0]+","+value+","+rgb[2];
    setState( "mqtt.0.pi_keller.funktionen.rgb",out);
});
on({id: "mqtt.0.stereoanlage.data.3", change: "ne"}, function (obj) {
    var value = obj.state.val;
    var rgb = getRGBVals();
    value = Math.round(value/1024*255);
    var out = rgb[0]+","+rgb[1]+","+value;
    setState( "mqtt.0.pi_keller.funktionen.rgb",out);
});
on({id: "mqtt.0.stereoanlage.data.10", change: "ne"}, function (obj) {
    var value = obj.state.val;
    setState( "mqtt.0.stereoanlage.data.11",0);
    setState( "mqtt.0.stereoanlage.data.12",0);
});
on({id: "mqtt.0.stereoanlage.data.11", change: "ne"}, function (obj) {
    var value = obj.state.val;
    setState( "mqtt.0.stereoanlage.data.10",0);
    setState( "mqtt.0.stereoanlage.data.12",0);
});
on({id: "mqtt.0.stereoanlage.data.12", change: "ne"}, function (obj) {
    var value = obj.state.val;
    setState( "mqtt.0.stereoanlage.data.10",0);
    setState( "mqtt.0.stereoanlage.data.11",0);
});

function getRGBVals(){
    return getState( "mqtt.0.pi_keller.funktionen.rgb").val.split(",");
}