import { Observable, takeUntil } from "rxjs";
import { RemotingService } from "./remoting.service";
import { Inject, inject } from "@angular/core";

export class RemoteObject<T> {
    private _value: T | null = null;
    private _keys: (string | number)[] = [];
    private _value$: Observable<T>

    constructor(private path: string, private remoting: RemotingService, private destroy$: Observable<void>, initial: T | null = null) {
        this._value$ = remoting.subscribe<T>(path, destroy$).pipe(takeUntil(destroy$));

        this.value$.subscribe({
            next: value => {
                this._value = value;

                if (typeof value === 'object' && value !== null)
                    this.subscribeObject()
            },
            complete: () => {
                if (typeof this._value === "object")
                    this.unsubscribeObject()
            }
        });

        if (initial !== null)
            this._value = initial
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
            let keyValue = (this._value as any)[key]

            const set = (x: any) => {
                console.log('set', `${this.path}.${key}`, x)
                this.remoting.set(`${this.path}.${key}`, x);
            }
            const get = () => keyValue

            Object.defineProperty(this._value, key, { set, get })

            this.remoting.subscribe<T>(`${this.path}.${key}`, this.destroy$)
                .pipe(takeUntil(this.destroy$))
                .subscribe(value => {
                    keyValue = value

                    if (typeof value === 'object') {
                        const childObject = new RemoteObject<any>(`${this.path}.${key}`, this.remoting, this.destroy$)
                        childObject.value$.subscribe(x => (this._value as any)[key] = x)
                    }
                });
        }
    }

    private unsubscribeObject() {
        for (let key of this._keys)
            RemotingService.instance.unsubscribe(`${this.path}.${key}`);
    }
}


export class RemotedObject {
    constructor(
        public object: any,
        public path: string,
        public destroy$: Observable<void>,
    ) { }
}


export function RemoteProperty() {
    return function (target: RemotedObject, propertyKey: string) {

        let value: any;
        const get = () => value;
        const set = (x: any) => RemotingService.instance.set(`${target.path}.${propertyKey}`, x);
        Object.defineProperty(target, propertyKey, { get, set })

        RemotingService.instance.subscribe(`${target.path}.${propertyKey}`, target.destroy$).subscribe((x: any) => {
            x.path = `${target.path}.${propertyKey}`;
            x.destroy$ = target.destroy$;
            value = x;
        })
    }
}