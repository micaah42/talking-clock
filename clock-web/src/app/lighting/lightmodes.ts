import { RemotedObject, RemoteProperty } from "../services/remoteobject"

export class LightMode {
    name: string = '';
}

export class StaticLight extends LightMode {
    color: string = '#000000';
}

export class WavingLight extends LightMode {
    a: string = '#000000';
    b: string = '#000000';
    length: number = 0;
    speed: number = 0;
}

export class Lighting {
    enabled: boolean = false;
    brightness: number = 1;
    mode: LightMode | null = null;
    staticLight: StaticLight = new StaticLight();
    wavingLight: WavingLight = new WavingLight();


}