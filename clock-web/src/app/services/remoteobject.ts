import { Observable, takeUntil } from "rxjs";
import { RemotingService } from "./remoting.service";
import { Inject, inject } from "@angular/core";

export class RemoteObject<T> {
    private _value: T | null = null;
    private _keys: (string | number)[] = [];
    private _value$: Observable<T>

    constructor(private path: string, private remoting: RemotingService, private destroy$: Observable<void>) {
        this._value$ = remoting.subscribe<T>(path, destroy$).pipe(takeUntil(destroy$));
        this.value$.subscribe({
            next: value => {
                this._value = value;

                if (typeof value === 'object')
                    this.subscribeObject()
            },
            complete: () => {
                if (typeof this._value === "object")
                    this.unsubscribeObject()
            }
        });
    }

    public get value$(): Observable<T> {
        return this._value$
    }

    public get value(): T | null {
        return this._value;
    }

    private subscribeObject() {
        delete (this._value as any)['__typeId'];
        delete (this._value as any)['__typeName'];

        const newKeys = Array.isArray(this._value) ? Array.from({ length: this._value.length }, (x, i) => i) : Object.keys(this._value as object)
        console.log(this.value, newKeys)

        if (this._keys === newKeys)
            return

        this._keys = newKeys;

        for (let key of this._keys) {
            console.log(`${this.path}.${key}`)
            this.remoting.subscribe<T>(`${this.path}.${key}`, this.destroy$)
                .pipe(takeUntil(this.destroy$))
                .subscribe(value => {
                    (this._value as any)[key] = value

                    if (typeof value === 'object') {
                        const childObject = new RemoteObject<any>(`${this.path}.${key}`, this.remoting, this.destroy$)
                        childObject.value$.subscribe(x => (this._value as any)[key] = x)
                    }
                });
        }
    }

    private unsubscribeObject() {
        for (let key of this._keys)
            this.remoting.unsubscribe(`${this.path}.${key}`);
    }
}


export class RemotedObject {
    constructor(
        public destroy$: Observable<void>,
        public path: string,
        public remoting: RemotingService = inject(RemotingService)
    ) { }

}


export function RemoteProperty() {
    return function (target: RemotedObject, propertyKey: string) {

        let value: any;
        const get = () => value;
        const set = (x: any) => target.remoting.set(`${target.path}.${propertyKey}`, x);
        Object.defineProperty(target, propertyKey, { get, set })

        target.remoting.subscribe(`${target.path}.${propertyKey}`, target.destroy$).subscribe((x: any) => {
            x.path = `${target.path}.${propertyKey}`;
            x.destroy$ = target.destroy$;
            value = x;
        })
    }
}