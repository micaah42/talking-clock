import { Observable, takeUntil } from "rxjs";
import { RemotingService } from "./remoting.service";

export class RemoteObject<T> {
    private _value: T | null = null;
    private _keys: (string | number)[] = [];

    constructor(private path: string, private remoting: RemotingService, private destroy$: Observable<void>) {
        remoting.subscribe<T>(path, destroy$).pipe(takeUntil(destroy$)).subscribe({
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

    public get value(): T | null {
        return this._value;
    }

    private subscribeObject() {
        delete (this._value as any)['__typeId'];
        const newKeys = Array.isArray(this._value) ? Array.from({ length: this._value.length }, (x, i) => i) : Object.keys(this._value as object)
        console.log(this.value, newKeys)

        if (this._keys === newKeys)
            return

        this._keys = newKeys;

        for (let key of this._keys) {
            console.log(`${this.path}.${key}`)
            this.remoting.subscribe<T>(`${this.path}.${key}`, this.destroy$)
                .pipe(takeUntil(this.destroy$))
                .subscribe(value => (this._value as any)[key] = value);
        }
    }

    private unsubscribeObject() {
        for (let key of this._keys)
            this.remoting.unsubscribe(`${this.path}.${key}`);
    }
}