import { Injectable } from '@angular/core';
import { webSocket, WebSocketSubject } from "rxjs/webSocket";
import { Subject } from "rxjs";
import { catchError } from "rxjs/operators";
import { WS_ENDPOINT } from "./remoting.service";

@Injectable({
  providedIn: 'root'
})
export class WebSocketService {
  private socket$?: WebSocketSubject<any>;
  private _buffer: any[] = [];
  private messagesSubject$ = new Subject();
  public isOpen: boolean = false;
  public messages$ = this.messagesSubject$.pipe(catchError(e => {
    console.error(e);
    throw e;
  }));

  public connect(): void {
    console.log(`connecting to '${WS_ENDPOINT}'...`);
    this.socket$ = webSocket({
      url: WS_ENDPOINT,
      openObserver: {
        next: () => {
          this.isOpen = true;
          console.log('connected!');
        }
      },
      closeObserver: {
        next: () => {
          this.isOpen = false;
        }
      }
    });
    this.messages$ = this.socket$.asObservable();
    this._buffer.forEach(msg => this.sendMessage(msg));
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
