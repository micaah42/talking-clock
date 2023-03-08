import { Component, OnInit } from '@angular/core';
import { RemotingService } from "./remoting/remoting.service";
import { Title } from "@angular/platform-browser";
import { WebSocketService } from './remoting/web-socket.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {
  title = 'web';

  constructor(private websocket: WebSocketService, private titleService: Title) {
    titleService.setTitle('Talking Clock');
  }

  ngOnInit(): void {
  }

  get connected(): boolean {
    return this.websocket.isOpen
  }
}
