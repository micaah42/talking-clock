import { Component, OnDestroy } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { MatCardModule } from '@angular/material/card';
import { MatSlideToggle } from '@angular/material/slide-toggle';
import { MatSliderModule } from '@angular/material/slider';
import { MatTabsModule } from '@angular/material/tabs';

import { ColorPickerModule } from 'ngx-color-picker'

import { LightMode, Lighting, StaticLight, WavingLight } from './lightmodes'
import { MatButtonModule } from '@angular/material/button';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatInputModule } from '@angular/material/input';
import { MatIconModule } from '@angular/material/icon';
import { Subject } from 'rxjs';
import { RemoteObject } from '../services/remoteobject';
import { RemotingService } from '../services/remoting.service';

@Component({
  selector: 'app-lighting',
  standalone: true,
  imports: [MatSlideToggle, MatSliderModule, MatCardModule, FormsModule, MatTabsModule, ColorPickerModule, MatButtonModule, MatFormFieldModule, MatInputModule, MatIconModule],
  templateUrl: './lighting.component.html',
  styleUrl: './lighting.component.scss'
})
export class LightingComponent implements OnDestroy {
  private destroy$ = new Subject<void>()
  lightsEnabled: boolean = true
  _brightness: number = 1

  set brightness(newBrightness) {
    console.log(newBrightness)
    this._brightness = newBrightness
  }
  get brightness() {
    return this._brightness
  }



  lighting: Lighting = new Lighting
  // lightingR: RemoteObject<Lighting>
  staticLight: RemoteObject<StaticLight>
  // wavingLight: RemoteObject<WavingLight>
  lightModes: (LightMode | any)[] = [];

  constructor(remoting: RemotingService) {
    new RemoteObject<Lighting>('lights', remoting, this.destroy$).value$.subscribe(x => this.lighting = x)
    this.staticLight = new RemoteObject<StaticLight>('lights.staticLight', remoting, this.destroy$)
    // this.wavingLight = new RemoteObject<WavingLight>('lights.wavingLight', remoting, this.destroy$)
    // this.lightModes = [this.staticLight, this.wavingLight];
  }

  public static stringify(x: any): string {
    return JSON.stringify(x)
  }
  keys(object: object): string[] {
    const keys = Object.keys(object)
    return keys.filter(x => x !== 'active' && x !== 'name' && x !== 'objectName');
  }
  typeof(value: any): string {
    return typeof value;
  }

  ngOnDestroy(): void {
    this.destroy$.next()
  }
}
