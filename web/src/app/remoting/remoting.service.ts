import {Injectable} from '@angular/core';
import {environment} from '../../environments/environment';
import {WebSocketService} from "./web-socket.service";
import {Subject, Subscription} from "rxjs";

export const WS_ENDPOINT = environment.wsEndpoint;

interface PendingPromise {
  resolve: ((value: any) => void);
  reject: ((value: any) => void);
  sent: Date;
}

export interface Info<T> {
  key: string;
  value: T;
}

@Injectable({
  providedIn: 'root'
})
export class RemotingService {
  private _counter = 1;
  private _pending: Map<number, PendingPromise> = new Map<number, PendingPromise>();
  private _notifications$: Subject<Info<unknown>> = new Subject<Info<unknown>>();
  private _notificationSubjects: Map<string, Subject<any>> = new Map<string, Subject<any>>();

  constructor(private websocket: WebSocketService) {
    this._notifications$.subscribe((notification: Info<unknown>) => {
      this._notificationSubjects.forEach((value, key) => {
        if (notification.key.match(key)) {
          value.next(notification);
        }
      })
    });

    this.websocket.connect();

    this.websocket.messages$.subscribe((msg: any) => {
      // handle answer (has id)
      if (msg.id) {
        const p = this._pending.get(msg.id);
        if (!p) {
          console.error('Unexpected answer:', msg);
          return;
        }

        if (!msg.error) {
          console.log(`handled '${msg.id}', in ${new Date().getTime() - p.sent.getTime()}ms`);
          p.resolve(msg.value);
        } else {
          console.warn(msg);
          p.reject(msg.error);
        }
        this._pending.delete(msg.id);
      }

      // handle notifications
      else if (msg.notifications) {
        msg.notifications.forEach((notification: Info<unknown>) => this._notifications$.next(notification));
      }

      // other messages are not expected
      else {
        console.error('received unknown message type:', msg);
      }
    });
  }

  public async cmd<T>(receiver: string, method: string, args: unknown[] = []): Promise<T> {
    const id = this._counter;
    this._counter++;

    const msg = {
      id: id,
      receiver: receiver,
      method: method,
      args: args
    };
    return new Promise<T>((resolve, reject) => {
      this._pending.set(id, {resolve: resolve, reject: reject, sent: new Date()});
      this.websocket.sendMessage(msg);
    });
  }

  public subscribe<T>(key: string, next: (info: Info<T>) => void): Subscription {
    if (!this._notificationSubjects.has(key)) {
      this._notificationSubjects.set(key, new Subject<T>());
    }

    // @ts-ignore
    return this._notificationSubjects.get(key).subscribe(next);
  }
}
