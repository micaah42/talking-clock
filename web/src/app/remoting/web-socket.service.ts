import { Injectable } from '@angular/core';
import { webSocket, WebSocketSubject } from "rxjs/webSocket";
import { interval, Observable, Subject } from "rxjs";
import { catchError, take, timeout } from "rxjs/operators";
import { WS_ENDPOINT } from "./remoting.service";

@Injectable({
  providedIn: 'root'
})
export class WebSocketService {
  private socket$!: WebSocketSubject<any>;
  private _messages$ !: Observable<any>;
  private _buffer: any[] = [];

  public isOpen: boolean = false;

  get messages$() {
    return this._messages$;
  }

  public connect(): void {
    console.log(`connecting to '${WS_ENDPOINT}'...`);

    this.socket$ = webSocket({
      url: WS_ENDPOINT,

      openObserver: {
        next: () => {
          this.isOpen = true;
          console.log('connected!');
        },
        error: (e) => {
          this.isOpen = false;
          console.warn('failed to connect:', e)
        }
      },
      closeObserver: {
        next: () => {
          this.isOpen = false;
        }
      }
    });

    this.socket$.subscribe({
      error: (e) => {
        console.error('socket closed:', e)
        interval(1000).pipe(take(1)).subscribe(() => this.connect())
      }
    })

    this._messages$ = this.socket$.asObservable();
    this._buffer.forEach(msg => this.sendMessage(msg));
    this._buffer = []
  }

  sendMessage(msg: any) {
    if (this.socket$)
      this.socket$.next(msg);
    else {
      this._buffer.push(msg);
    }
  }

  close() {
    if (this.socket$)
      this.socket$.complete();
  }
}
