import { RemotedObject, RemoteProperty } from "../services/remoteobject"

export class LightMode extends RemotedObject {
    @RemoteProperty()
    name: string = '';
}

export class StaticLight extends LightMode {
    @RemoteProperty()
    color: string = '#000000';
}

export class WavingLight extends LightMode {
    @RemoteProperty()
    a: string = '#000000';

    @RemoteProperty()
    b: string = '#000000';

    @RemoteProperty()
    length: number = 0;

    @RemoteProperty()
    speed: number = 0;

}